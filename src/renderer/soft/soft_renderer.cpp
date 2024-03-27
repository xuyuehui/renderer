#include "soft_renderer.h"
#include "frame_buffer.h"
#include "../../platform/platform.h"
#include "rasterizer.h"
#include "font.h"
#include "renderworld_soft.h"

#include "shadermanager_soft.h"
#include "../modelmanager.h"
#include "../TextureManager.h"
#include "../material.h"
#include "shader_soft.h"
#include "../model.h"

#include "../../utility/str.h"

namespace CG {

typedef struct bbox_s {
    int minX;
    int minY;
    int maxX;
    int maxY;
}bbox_t;

static inline byte Float2ByteColor(float v) {
    return (byte)(clamp(v, 0.0f, 1.0f) * 255);
}

// https://www.khronos.org/registry/OpenGL/specs/es/2.0/es_full_spec_2.0.pdf subsection 2.12.1
static inline Vec3 ViewportMapping(int width, int height, const Vec4 &pos) {
    Vec3 point;

    // [-1, 1] -> [0, w]
    point.x = (pos.x + 1) * 0.5f * width;

    // [-1, 1] -> [0, h]
    point.y = (pos.y + 1) * 0.5f * height;

    // [-1, 1] ->[0, 1]
    point.z = (pos.z + 1) * 0.5f;

    return point;
}

static inline bool IsVertexVisible(const Vec4 &v) {
    return fabs(v.x) <= v.w && fabs(v.y) <= v.w && fabs(v.z) <= v.w;
}

typedef enum {
    POSITIVE_W,
    POSITIVE_X,
    NEGATIVE_X,
    POSITIVE_Y,
    NEGATIVE_Y,
    POSITIVE_Z,
    NEGATIVE_Z,
} clipPlane_t;

static inline bool IsInsidePlane(const Vec4 &pos, clipPlane_t plane) {
    switch (plane) {
    case POSITIVE_W:
        return pos.w >= Math::EPSILON;
    case POSITIVE_X:
        return pos.x <= pos.w;
    case NEGATIVE_X:
        return pos.x >= -pos.w;
    case POSITIVE_Y:
        return pos.y <= pos.w;
    case NEGATIVE_Y:
        return pos.y >= -pos.w;
    case POSITIVE_Z:
        return pos.z <= pos.w;
    case NEGATIVE_Z:
        return pos.z >= -pos.w;
    }

    assert(false);
    return false;
}

static inline float GetIntersetRatio(const Vec4 &prev, const Vec4 &curr, clipPlane_t plane) {
    switch (plane) {
    case POSITIVE_W:
        return (prev.w - Math::EPSILON) / (prev.w - curr.w);
    case POSITIVE_X:
        return (prev.w - prev.x) / ((prev.w - prev.x) - (curr.w - curr.x));
    case NEGATIVE_X:
        return (prev.w + prev.x) / ((prev.w + prev.x) - (curr.w + curr.x));
    case POSITIVE_Y:
        return (prev.w - prev.y) / ((prev.w - prev.y) - (curr.w - curr.y));
    case NEGATIVE_Y:
        return (prev.w + prev.y) / ((prev.w + prev.y) - (curr.w + curr.y));
    case POSITIVE_Z:
        return (prev.w - prev.z) / ((prev.w - prev.z) - (curr.w - curr.z));
    case NEGATIVE_Z:
        return (prev.w + prev.z) / ((prev.w + prev.z) - (curr.w + curr.z));
    }

    return .0f;
}

static inline int ClipAgainstPlane(clipPlane_t plane, Vec4 inCoords[MAX_VARYINGS], ishaderVarying_t *inVaryings[MAX_VARYINGS], int numInVert, Vec4 outCoords[MAX_VARYINGS], ishaderVarying_t *outVaryings[MAX_VARYINGS], IProgram *program) {
    int outNumVert = 0;

    for (int i = 0; i < numInVert; i++) {
        int currIndex = i;
        int prevIndex = (i - 1 + numInVert) % numInVert;

        Vec4 &currCoord = inCoords[currIndex];
        Vec4 &prevCoord = inCoords[prevIndex];
        ishaderVarying_t *currVarying = inVaryings[currIndex];
        ishaderVarying_t *prevVarying = inVaryings[prevIndex];

        // 判断当前边的两个点与裁截面的关系
        bool isCurrInside = IsInsidePlane(currCoord, plane);
        bool isPrevInside = IsInsidePlane(prevCoord, plane);

        // 一个在外，一个在内，计算线段和面相交的点，取代被裁掉的点
        if (isCurrInside != isPrevInside) {
            float ratio = GetIntersetRatio(prevCoord, currCoord, plane);
            program->Interpolate(prevVarying, currVarying, ratio, outVaryings[outNumVert]);
            outCoords[outNumVert] = Vec4::Lerp(prevCoord, currCoord, ratio);

            outNumVert++;
        }

        if (isCurrInside) {
            program->CopyFrom(currVarying, outVaryings[outNumVert]);
            outCoords[outNumVert] = currCoord;

            outNumVert++;
        }

        assert(outNumVert <= MAX_VARYINGS);
    }

    return outNumVert;
}

// 齐次坐标空间下的三角形裁剪
// https://zhuanlan.zhihu.com/p/162190576
static inline int ClipTriangle(Vec4 inCoords[MAX_VARYINGS], ishaderVarying_t *inVaryings[MAX_VARYINGS], Vec4 outCoords[MAX_VARYINGS], ishaderVarying_t *outVaryings[MAX_VARYINGS], IProgram *program) {
    bool visible0 = IsVertexVisible(inCoords[0]);
    bool visible1 = IsVertexVisible(inCoords[1]);
    bool visible2 = IsVertexVisible(inCoords[2]);

    // 都是可见，则不需要裁剪
    if (visible0 && visible1 && visible2) {
        outCoords[0] = inCoords[0];
        outCoords[1] = inCoords[1];
        outCoords[2] = inCoords[2];

        program->CopyFrom(inVaryings[0], outVaryings[0]);
        program->CopyFrom(inVaryings[1], outVaryings[1]);
        program->CopyFrom(inVaryings[2], outVaryings[2]);
        return 3;
    }

    int numVert = 3;

    numVert = ClipAgainstPlane(POSITIVE_W, inCoords, inVaryings, numVert, outCoords, outVaryings, program);
    if (numVert < 3) {
        return 0;
    }

    numVert = ClipAgainstPlane(POSITIVE_X, outCoords, outVaryings, numVert, inCoords, inVaryings, program);
    if (numVert < 3) {
        return 0;
    }

    numVert = ClipAgainstPlane(NEGATIVE_X, inCoords, inVaryings, numVert, outCoords, outVaryings, program);
    if (numVert < 3) {
        return 0;
    }

    numVert = ClipAgainstPlane(POSITIVE_Y, outCoords, outVaryings, numVert, inCoords, inVaryings, program);
    if (numVert < 3) {
        return 0;
    }

    numVert = ClipAgainstPlane(NEGATIVE_Y, inCoords, inVaryings, numVert, outCoords, outVaryings, program);
    if (numVert < 3) {
        return 0;
    }

    numVert = ClipAgainstPlane(POSITIVE_Z, outCoords, outVaryings, numVert, inCoords, inVaryings, program);
    if (numVert < 3) {
        return 0;
    }

    numVert = ClipAgainstPlane(NEGATIVE_Z, inCoords, inVaryings, numVert, outCoords, outVaryings, program);
    if (numVert < 3) {
        return 0;
    }

    return numVert;
}

static inline bool isBackface_1(const Vec4 &v0, const Vec4 &v1, const Vec4 &v2) {
    const Vec3 &e1 = (v1 - v0).ToVec3();
    const Vec3 &e2 = (v2 - v0).ToVec3();

    return e1.Cross(e2).z <= 0.0f;
}

// https://www.khronos.org/registry/OpenGL/specs/es/2.0/es_full_spec_2.0.pdf subsection 3.5.1
static inline bool isBackface_0(const Vec4 &v0, const Vec4 &v1, const Vec4 &v2) {
    float signedArea = v0.x * v1.y - v0.y * v1.x + v1.x * v2.y - v1.y * v2.x + v2.x * v0.y - v2.y * v0.x;

    return signedArea <= 0.0f;
}

// 获取三角形三个顶点覆盖的Bounding Box
static inline bbox_t FindBoundingBox(Vec3 *points, int width, int height) {
    bbox_t bbox;

    bbox.minX = max(min(points[0].x, min(points[1].x, points[2].x)), 0);
    bbox.maxX = min(max(points[0].x, max(points[1].x, points[2].x)), width - 1);
    bbox.minY = max(min(points[0].y, min(points[1].y, points[2].y)), 0);
    bbox.maxY = min(max(points[0].y, max(points[1].y, points[2].y)), height - 1);

    return bbox;
}

static inline float InterpolateDepth(Vec3 *points, const Vec3 &weights) {
    return points[0].z * weights.x + points[1].z * weights.y + points[2].z * weights.z;
}

/*
    * for barycentric coordinates, see
    * http://blackpawn.com/texts/pointinpoly/
    *
    * solve
    *     P = A + s * AB + t * AC  -->  AP = s * AB + t * AC
    * then
    *     s = (AC.y * AP.x - AC.x * AP.y) / (AB.x * AC.y - AB.y * AC.x)
    *     t = (AB.x * AP.y - AB.y * AP.x) / (AB.x * AC.y - AB.y * AC.x)
    *
    * notice
    *     P = A + s * AB + t * AC
    *       = A + s * (B - A) + t * (C - A)
    *       = (1 - s - t) * A + s * B + t * C
    * then
    *     weight_A = 1 - s - t
    *     weight_B = s
    *     weight_C = t
    */
static inline Vec3 CalculateWeight(Vec2 abc[3], Vec2 &p) {
    Vec2 ab = abc[1] - abc[0];
    Vec2 ac = abc[2] - abc[0];
    Vec2 ap = p - abc[0];
    float factor = 1.0f / (ab.x * ac.y - ab.y * ac.x);
    float s = (ac.y * ap.x - ac.x * ap.y) * factor;
    float t = (ab.x * ap.y - ab.y * ap.x) * factor;

    return Vec3(1 - s - t, s, t);
}

inline void PerspectiveDivision(Vec4 &v) {
    float rw = 1.0f / v.w;
    v.x *= rw;
    v.y *= rw;
    v.z *= rw;
}

inline void DrawFragment(FrameBuffer *frameBuffer, ishaderVarying_t **varyings, IProgram *program, int index, bool backface, float depth) {
    bool discard = false;
    Vec4 color = dynamic_cast<Shader_Soft *>(program->shader)->Fragment(program->shaderVarying, program->uniforms, backface, discard);
    if (discard) {
        return;
    }

    color = Math::Saturate(color);

    byte *colorBuffer = frameBuffer->GetColorBuffer();

    if (program->enableBlend) {
        // out_color = src_color * src_alpha + dst_color * (1 - src_alpha)
        byte dstR = colorBuffer[index * 3 + 0];
        byte dstG = colorBuffer[index * 3 + 1];
        byte dstB = colorBuffer[index * 3 + 2];

        color.x = color.x * color.w + dstR * (1 - color.w);
        color.y = color.y * color.w + dstG * (1 - color.w);
        color.z = color.z * color.w + dstB * (1 - color.w);
    }

    colorBuffer[index * 3 + 0] = Float2ByteColor(color.x);
    colorBuffer[index * 3 + 1] = Float2ByteColor(color.y);
    colorBuffer[index * 3 + 2] = Float2ByteColor(color.z);

    frameBuffer->GetDepthBuffer()[index] = depth;
}

static inline bool DrawTriangle_0(FrameBuffer *frameBuffer, Vec3 *points, float *recipW, ishaderVarying_t **varyings, IProgram *program, bool backface) {
    Vec2 screenCoords[3];
    for (int i = 0; i < 3; ++i) {
        screenCoords[i] = points[i].ToVec2();
    }

    // perform rasterization
    bbox_t bbox = FindBoundingBox(points, frameBuffer->GetWidth(), frameBuffer->GetHeight());
    for (int x = bbox.minX; x <= bbox.maxX; x++) {
        for (int y = bbox.minY; y <= bbox.maxY; y++) {
            Vec2 point(x + 0.5f, y + 0.5f);
            Vec3 weights = CalculateWeight(screenCoords, point);
            bool w0 = weights.x > -Math::EPSILON;
            bool w1 = weights.y > -Math::EPSILON;
            bool w2 = weights.z > -Math::EPSILON;

            // 判断点是否在三角形里
            if (!w0 || !w1 || !w2) {
                continue;
            }

            int depthIndex = y * frameBuffer->GetWidth() + x;
            float depth = InterpolateDepth(points, weights);
                
            // 深度测试
            if (depth > frameBuffer->GetDepthBuffer()[depthIndex]) {
                continue;
            }

            // https://www.scratchapixel.com/lessons/3d-basic-rendering/rasterization-practical-implementation/perspective-correct-interpolation-vertex-attributes.html
            // 根据重心坐标插值计算片元属性
            program->Interpolate(varyings, weights, recipW, program->shaderVarying);

            byte *colorBuffer = frameBuffer->GetColorBuffer();
            int index = (frameBuffer->GetWidth() * y + x);

            DrawFragment(frameBuffer, varyings, program, index, backface, depth);
        }
    }

    return true;
}

static inline bool RasterizeTriangle(FrameBuffer *frameBuffer, Vec4 *verts, ishaderVarying_t **varyings, IProgram *program) {
    // perspective division
    for (int i = 0; i < 3; i++) {
        PerspectiveDivision(verts[i]);
    }

    // back face culling
    bool backface = isBackface_0(verts[0], verts[1], verts[2]);
    if (!program->doubleSided && backface) {
        return false;
    }

    float recipW[3];

    // precompute reciprocals of w
    for (int i = 0; i < 3; i++) {
        recipW[i] = 1.0f / verts[i].w;
    }

    // viewport mapping
    Vec3 points[3];
   
    for (int i = 0; i < 3; ++i) {
        points[i] = ViewportMapping(frameBuffer->GetWidth(), frameBuffer->GetHeight(), verts[i]);
    }

    // draw triangle
    DrawTriangle_0(frameBuffer, points, recipW, varyings, program, backface);

    return false;
}

SoftRenderer::SoftRenderer() : bufferIndex(0), needUpdated(false), window(NULL), shaderManager(NULL), modelManager(NULL), textureManager(NULL), renderFlags(0), antiAlias(AA_DEFAULT) {
    buffers[0] = NULL;
    buffers[1] = NULL;
}

SoftRenderer::~SoftRenderer() {
    delete buffers[0];
    delete buffers[1];

    if (shaderManager != NULL) {
        shaderManager->Shutdown();
        delete shaderManager;
    }


    if (modelManager != NULL) {
        modelManager->Shutdown();
        delete modelManager;
    }

    if (textureManager != NULL) {
        textureManager->Shutdown();
        delete textureManager;
    }
}

void SoftRenderer::Init(Window* window) {
    if (buffers[0] != NULL) {
        delete buffers[0];
    }

    if (buffers[1] != NULL) {
        delete buffers[1];
    }

    this->window = window;

    int width = 0;
    int height = 0;

    window->GetSize(width, height);

    buffers[0] = new FrameBuffer(width, height);
    buffers[1] = new FrameBuffer(width, height);

    shaderManager = new ShaderManager_Soft();
    shaderManager->Init();

    modelManager = new ModelManager();
    modelManager->Init();

    textureManager = new TextureManager();
    textureManager->Init();
}

void SoftRenderer::ClearColorBuffer(const Vec3 &color) {
    GetBackFrameBuffer()->ClearColorBuffer(color);
}

void SoftRenderer::ClearDepthBuffer(float depth) {
    GetBackFrameBuffer()->ClearDepthBuffer(depth);
}

void SoftRenderer::SwapBuffer() {
    bufferIndex ^= 1;
    GetBackFrameBuffer()->ClearDepthBuffer(1.0f);
    needUpdated = true;

    FrameBuffer *frameBuffer = GetFrontFrameBuffer();
        
    window->SwapBuffer(frameBuffer->GetColorBuffer(), frameBuffer->GetWidth(), frameBuffer->GetHeight());
}

void SoftRenderer::DrawLine(const Vec2& s, const Vec2& e, const color_t& color, float depth) {
    FrameBuffer* frameBuffer = GetBackFrameBuffer();
    R_DrawLine(*frameBuffer, s, e, color, depth);
}

void SoftRenderer::DrawText(const char* text, const Vec2& pos, float size, const color_t& color, float gap) {
    size_t len = strlen(text);
    FrameBuffer * frameBuffer = GetBackFrameBuffer();
    float x = pos.x;
    float y = pos.y;
    const float ratio = 2.0f;

    for (int i = 0; i < len; ++i) {
        if (text[i] >= 'a' && text[i] <= 'z') {
            Font::R_DrawCharacter(*frameBuffer, x, y, (FONT_CHARACTER)(CHAR_A + text[i] - 'a'), size, color, ratio);
        }
        else if (text[i] >= 'A' && text[i] <= 'Z') {
            Font::R_DrawCharacter(*frameBuffer, x, y, (FONT_CHARACTER)(CHAR_A + text[i] - 'A'), size, color, ratio);
        }
        else if (text[i] == '-') {
            Font::R_DrawCharacter(*frameBuffer, x, y, CHAR_DASH, size, color, ratio);
        }
        else if (text[i] >= '0' && text[i] <= '9') {
            Font::R_DrawCharacter(*frameBuffer, x, y, (FONT_CHARACTER)(CHAR_0 + text[i] - '0'), size, color, ratio);
        }

        x += size * (1.0f + gap);
    }
}

RenderWorld *SoftRenderer::CreateRenderWorld(shadingMode_t mode) {
    switch (mode) {
    case SHADING_BLINN:
        return new RenderWorldBlinn_Soft(this);
    case SHADING_PBRM:
        return new RenderWorldPbrm_Soft(this);
    case SHADING_PBRS:
        return new RenderWorldPbrs_Soft(this);
    default:
        assert(false);
        return NULL;
    }
}

void SoftRenderer::DrawSurface(const RenderWorld *renderWorld, const modelSurface_t *surface, bool shadowPass, FrameBuffer *dst) {
    if (!surface->geometry) {
        return;
    }

    FrameBuffer *frameBuffer = dst ? dst : this->GetBackFrameBuffer();
    IProgram *program = renderWorld->GetProgram();
    program->SetupMaterial(surface->material);

    program->uniforms->shadowPass = shadowPass;

    Shader_Soft *shader = dynamic_cast<Shader_Soft *>(program->shader);

    for (int i = 0, tidx = 0; i < surface->geometry->numIndexes; i += 3, tidx++) {
        program->SetupVertex(surface->geometry->verts[surface->geometry->indexes[i + 0]], program->attribs[0], program->inVaryings[0]);
        program->SetupVertex(surface->geometry->verts[surface->geometry->indexes[i + 1]], program->attribs[1], program->inVaryings[1]);
        program->SetupVertex(surface->geometry->verts[surface->geometry->indexes[i + 2]], program->attribs[2], program->inVaryings[2]);

        // vertex processing
        program->inCoords[0] = shader->Vertex(program->attribs[0], program->uniforms, program->inVaryings[0]);
        program->inCoords[1] = shader->Vertex(program->attribs[1], program->uniforms, program->inVaryings[1]);
        program->inCoords[2] = shader->Vertex(program->attribs[2], program->uniforms, program->inVaryings[2]);

        // cliping triangle
        int numVerices = ClipTriangle(program->inCoords, program->inVaryings, program->outCoords, program->outVaryings, program);
        if (numVerices < 3) {
            continue;
        }

        // triangle assembly
        for (int i = 0; i < numVerices-2;) {

            // rasterize triangle
            bool isCulled = RasterizeTriangle(frameBuffer, &program->outCoords[i], &program->outVaryings[i], program);
            if (isCulled) {
                break;
            }

            i += 3;
        }
    }
}

}
