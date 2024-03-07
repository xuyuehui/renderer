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
    static inline void ViewportMapping(int width, int height, Vec4 &pos) {
        // [-1, 1] -> [0, w]
        pos.x = (pos.x + 1) * 0.5f * width;

        // [-1, 1] -> [0, h]
        pos.y = (pos.y + 1) * 0.5f * height;

        // [-1, 1] ->[0, 1]
        pos.z = (pos.z + 1) * 0.5f;
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
            return pos.w >= EPSILON;
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
            return (prev.w - EPSILON) / (prev.w - curr.w);
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

    static inline int ClipAgainstPlane(clipPlane_t plane, ishaderVarying_t *inVertices[MAX_VARYINGS], int numInVert, ishaderVarying_t *outVeritces[MAX_VARYINGS], IProgram *program) {
        int outNumVert = 0;

        for (int i = 0; i < numInVert; i++) {
            int currIndex = i;
            int prevIndex = (i - 1 + numInVert) % numInVert;

            ishaderVarying_t *curr = inVertices[currIndex];
            ishaderVarying_t *prev = inVertices[prevIndex];

            // 判断当前边的两个点与裁截面的关系
            bool isCurrInside = IsInsidePlane(curr->position, plane);
            bool isPrevInside = IsInsidePlane(prev->position, plane);

            // 一个在外，一个在内，计算线段和面相交的点，取代被裁掉的点
            if (isCurrInside != isPrevInside) {
                float ratio = GetIntersetRatio(prev->position, curr->position, plane);
                program->Interpolate(prev, curr, ratio, outVeritces[outNumVert]);

                outNumVert++;
            }

            if (isCurrInside) {
                program->CopyFrom(curr, outVeritces[outNumVert]);
                outNumVert++;
            }

            assert(outNumVert <= MAX_VARYINGS);
        }

        return outNumVert;
    }

    // 齐次坐标空间下的三角形裁剪
    // https://zhuanlan.zhihu.com/p/162190576
    static inline int ClipTriangle(ishaderVarying_t *inVertices[MAX_VARYINGS], ishaderVarying_t *outVeritces[MAX_VARYINGS], IProgram *program) {
        bool visible0 = IsVertexVisible(inVertices[0]->position);
        bool visible1 = IsVertexVisible(inVertices[1]->position);
        bool visible2 = IsVertexVisible(inVertices[2]->position);

        // 都是可见，则不需要裁剪
        if (visible0 && visible1 && visible2) {
            program->CopyFrom(inVertices[0], outVeritces[0]);
            program->CopyFrom(inVertices[1], outVeritces[1]);
            program->CopyFrom(inVertices[2], outVeritces[2]);
            return 3;
        }

        int numVert = 3;

        numVert = ClipAgainstPlane(POSITIVE_W, inVertices, numVert, outVeritces, program);
        if (numVert < 3) {
            return 0;
        }

        numVert = ClipAgainstPlane(POSITIVE_X, outVeritces, numVert, inVertices, program);
        if (numVert < 3) {
            return 0;
        }

        numVert = ClipAgainstPlane(NEGATIVE_X, inVertices, numVert, outVeritces, program);
        if (numVert < 3) {
            return 0;
        }

        numVert = ClipAgainstPlane(POSITIVE_Y, outVeritces, numVert, inVertices, program);
        if (numVert < 3) {
            return 0;
        }

        numVert = ClipAgainstPlane(NEGATIVE_Y, inVertices, numVert, outVeritces, program);
        if (numVert < 3) {
            return 0;
        }

        numVert = ClipAgainstPlane(POSITIVE_Z, outVeritces, numVert, inVertices, program);
        if (numVert < 3) {
            return 0;
        }

        numVert = ClipAgainstPlane(NEGATIVE_Z, inVertices, numVert, outVeritces, program);
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
    static inline bbox_t FindBoundingBox(ishaderVarying_t **verts, int width, int height) {
        bbox_t bbox;

        bbox.minX = max(min(verts[0]->position.x, min(verts[1]->position.x, verts[2]->position.x)), 0);
        bbox.maxX = min(max(verts[0]->position.x, max(verts[1]->position.x, verts[2]->position.x)), width - 1);
        bbox.minY = max(min(verts[0]->position.y, min(verts[1]->position.y, verts[2]->position.y)), 0);
        bbox.maxY = min(max(verts[0]->position.y, max(verts[1]->position.y, verts[2]->position.y)), height - 1);

        return bbox;
    }

    static inline float InterpolateDepth(ishaderVarying_t **verts, const Vec3 &weights) {
        return verts[0]->position.z * weights.x + verts[1]->position.z * weights.y + verts[2]->position.z * weights.z;
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

    inline void DrawPixel(FrameBuffer *frameBuffer, int x, int y, const Vec4 &color) {
        byte *colorBuffer = frameBuffer->GetColorBuffer();
        int colorPos = (frameBuffer->GetWidth() * y + x) * 3;

        colorBuffer[colorPos + 0] = Float2ByteColor(color.x);
        colorBuffer[colorPos + 1] = Float2ByteColor(color.y);
        colorBuffer[colorPos + 2] = Float2ByteColor(color.z);
    }

    static inline bool DrawTriangle_0(FrameBuffer *frameBuffer, ishaderVarying_t **verts, IProgram *program) {
        Vec2 screenCoords[3];
        for (int i = 0; i < 3; ++i) {
            screenCoords[i] = verts[i]->position.ToVec2();
        }

        // perform rasterization
        bbox_t bbox = FindBoundingBox(verts, frameBuffer->GetWidth(), frameBuffer->GetHeight());
        for (int x = bbox.minX; x <= bbox.maxX; x++) {
            for (int y = bbox.minY; y <= bbox.maxY; y++) {
                Vec2 point(x + 0.5f, y + 0.5f);
                Vec3 weights = CalculateWeight(screenCoords, point);
                bool w0 = weights.x > -EPSILON;
                bool w1 = weights.y > -EPSILON;
                bool w2 = weights.z > -EPSILON;

                // 判断点是否在三角形里
                if (!w0 || !w1 || !w2) {
                    continue;
                }

                int depthIndex = y * frameBuffer->GetWidth() + x;
                float depth = InterpolateDepth(verts, weights);
                
                // 深度测试
                if (depth > frameBuffer->GetDepthBuffer()[depthIndex]) {
                    continue;
                }

                // https://www.scratchapixel.com/lessons/3d-basic-rendering/rasterization-practical-implementation/perspective-correct-interpolation-vertex-attributes.html
                // 根据重心坐标插值计算片元属性
                program->Interpolate(verts, weights, program->shaderVarying);

                fragmentArgs_t fa;

                fa.varying = program->shaderVarying;
                memcpy(fa.textures, program->textures, sizeof(program->textures));

                Vec4 color = dynamic_cast<Shader_Soft *>(program->shader)->Fragment(&fa);

                DrawPixel(frameBuffer, x, y, color);
            }
        }

        return true;
    }

    static inline bool RasterizeTriangle(FrameBuffer *frameBuffer, ishaderVarying_t** verts, IProgram *program) {
        // perspective division
        for (int i = 0; i < 3; i++) {
            PerspectiveDivision(verts[i]->position);
        }

        // back face culling
        bool backface = isBackface_0(verts[0]->position, verts[1]->position, verts[2]->position);
        if (!program->doubleSided && backface) {
            return false;
        }

        // precompute reciprocals of w
        for (int i = 0; i < 3; i++) {
            verts[i]->position.w = 1.0f / verts[i]->position.w;
        }

        // viewport mapping
        for (int i = 0; i < 3; ++i) {
            ViewportMapping(frameBuffer->GetWidth(), frameBuffer->GetHeight(), verts[i]->position);
        }

        // draw triangle
        DrawTriangle_0(frameBuffer, verts, program);

        return false;
    }

    SoftRenderer::SoftRenderer() : bufferIndex(0), needUpdated(false), window(NULL), shaderManager(NULL), modelManager(NULL), textureManager(NULL), defaultMat(NULL), defaultShader(NULL), renderFlags(0), antiAlias(AA_DEFAULT) {
        buffers[0] = NULL;
        buffers[1] = NULL;

        program = new ProgramLocal();
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

        delete defaultMat;
        delete program;
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

        defaultShader = GetShaderManager()->LoadShader("internal/unlit");

        defaultMat = new Material();
        defaultMat->shader = defaultShader;
        defaultMat->SetRenderFlags(RF_DEPTH_TEST);
    }

    void SoftRenderer::ClearColorBuffer(const rgb& color) {
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

    RenderWorld *SoftRenderer::CreateRenderWorld() {
        return new RenderWorld_Soft(this);
    }

    void SoftRenderer::DrawSurface(const modelSurface_t *surface, const drawSurfaceContext_t &context) {
        FrameBuffer *frameBuffer = this->GetBackFrameBuffer();

        const Material *material = surface->material != NULL ? surface->material : defaultMat;
        program->shader = dynamic_cast<Shader_Soft *>(material->shader != NULL ? material->shader : defaultShader);
        program->textures[0] = material->albedo;
        program->textures[1] = material->diffuse;
        program->textures[2] = material->specular;
        program->textures[3] = material->normal;

        for (int i = 0, tidx = 0; i < surface->geometry->numIndexes; i += 3, tidx++) {
            program->Setup(program->inVaryings[0], surface->geometry->verts[surface->geometry->indexes[i + 0]], context);
            program->Setup(program->inVaryings[1], surface->geometry->verts[surface->geometry->indexes[i + 1]], context);
            program->Setup(program->inVaryings[2], surface->geometry->verts[surface->geometry->indexes[i + 2]], context);

            // Vertex Processing
            dynamic_cast<Shader_Soft *>(program->shader)->Vertex(program->inVaryings[0], program->outVaryings[0]);
            dynamic_cast<Shader_Soft *>(program->shader)->Vertex(program->inVaryings[1], program->outVaryings[1]);
            dynamic_cast<Shader_Soft *>(program->shader)->Vertex(program->inVaryings[2], program->outVaryings[2]);

            // Cliping Triangle
            int numVerices = ClipTriangle(program->outVaryings, program->inVaryings, program);
            if (numVerices < 3) {
                continue;
            }

            // Rasterize Triangle
            for (int i = 0; i < numVerices-2;) {
                bool isCulled = RasterizeTriangle(frameBuffer, &program->inVaryings[i], program);
                if (isCulled) {
                    break;
                }

                i += 3;
            }
        }
    }
}
