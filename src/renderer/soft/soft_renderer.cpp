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
    static inline vdata_t VDATA(const srfTriangles_t *triangles, int vertIndex, const Mat4 &model, const Mat4 &view, const Mat4 &proj, const Mat4 &vp, const Mat4 &mvp, const Mat4 &modelInvTranspose) {
        vdata_t vd;

        vd.modelMat = model;
        vd.viewMat = view;
        vd.projMat = proj;
        vd.vpMat = vp;
        vd.mvpMat = mvp;
        vd.modelInvTransposeMat = modelInvTranspose;

        vd.position = triangles->verts[triangles->indexes[vertIndex]].xyz;
        vd.normal = triangles->verts[triangles->indexes[vertIndex]].normal;
        vd.texcoord = triangles->verts[triangles->indexes[vertIndex]].st;
        vd.color = triangles->verts[triangles->indexes[vertIndex]].color;
        vd.tangent = triangles->verts[triangles->indexes[vertIndex]].tangent;

        return vd;
    }

    static inline float ScreenMapping_X(float x, int width) {
        return floor((x * 0.5f + 0.5f) * width);
    }

    static inline float ScreenMapping_Y(float y, int height) {
        return floor((y * 0.5f + 0.5f) * height);
    }

    static inline byte Float2ByteColor(float v) {
        return (byte)(clamp(v, 0.0f, 1.0f) * 255);
    }

    SoftRenderer::SoftRenderer() : bufferIndex(0), needUpdated(false), window(NULL), shaderManager(NULL), modelManager(NULL), textureManager(NULL), defaultMat(NULL), defaultShader(NULL), renderFlags(0), antiAlias(AA_DEFAULT) {
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

        delete defaultMat;
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
        defaultMat->SetRenderFlags(RF_BACK_FACE_CULLING | RF_DEPTH_TEST);
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
        
        window->SwapBuffer();
    }

    bool SoftRenderer::GetColorBufferDesc(renderTargetDesc_t& rtd) {
        if (!needUpdated) {
            return false;
        }

        FrameBuffer * frameBuffer = GetFrontFrameBuffer();
        rtd.bitCount = 24;
        rtd.width = frameBuffer->GetWidth();
        rtd.height = frameBuffer->GetHeight();
        rtd.data = frameBuffer->GetColorBuffer();

        needUpdated = false;

        return true;
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
        Shader_Soft *shader = dynamic_cast<Shader_Soft *>(material->shader != NULL ? material->shader : defaultShader);

        Mat4 vp = context.proj * context.view;
        Mat4 mvpMat = context.proj * context.view * context.model;
        Mat4 modelInvTransport = context.model.Inverse().Transport();

        for (int i = 0, tidx = 0; i < surface->geometry->numIndexes; i += 3, tidx++) {

            // Vertex Processing
            v2f_t v0 = shader->Vertex(VDATA(surface->geometry, i, context.model, context.view, context.proj, vp, mvpMat, modelInvTransport));
            v2f_t v1 = shader->Vertex(VDATA(surface->geometry, i + 1, context.model, context.view, context.proj, vp, mvpMat, modelInvTransport));
            v2f_t v2 = shader->Vertex(VDATA(surface->geometry, i + 2, context.model, context.view, context.proj, vp, mvpMat, modelInvTransport));

            // Perspective Division
            Math::PerspectiveDivision(v0.position);
            Math::PerspectiveDivision(v1.position);
            Math::PerspectiveDivision(v2.position);

            // Screen Clipping
            if ((v0.position.x < -1.0f && v1.position.x < -1.0f && v2.position.x < -1.0f) ||
                (v0.position.x >  1.0f && v1.position.x >  1.0f && v2.position.x >  1.0f) ||
                (v0.position.y < -1.0f && v1.position.y < -1.0f && v2.position.y < -1.0f) ||
                (v0.position.y >  1.0f && v1.position.y >  1.0f && v2.position.y >  1.0f) ||
                (v0.position.z <  0.0f && v1.position.z <  0.0f && v2.position.z <  0.0f) ||
                (v0.position.z >  1.0f && v1.position.z >  1.0f && v2.position.z >  1.0f)) {
                continue;
            }

            // Back Face Culling
            if (this->HasRenderFlags(RF_WIREFRAME_MODE) || material->HasRenderFlags(RF_WIREFRAME_MODE | RF_BACK_FACE_CULLING)) {
                const Vec3 &e1 = (v1.position - v0.position).ToVec3();
                const Vec3 &e2 = (v2.position - v0.position).ToVec3();

                // 三角形法线的z值为负，代表和摄像机方向一直，默认为-z，则为背面
                if (e1.Cross(e2).z < 0.0f) {
                    continue;
                }
            }

            v0.position.x = ScreenMapping_X(v0.position.x, frameBuffer->GetWidth());
            v1.position.x = ScreenMapping_X(v1.position.x, frameBuffer->GetWidth());
            v2.position.x = ScreenMapping_X(v2.position.x, frameBuffer->GetWidth());
            v0.position.y = ScreenMapping_Y(v0.position.y, frameBuffer->GetHeight());
            v1.position.y = ScreenMapping_Y(v1.position.y, frameBuffer->GetHeight());
            v2.position.y = ScreenMapping_Y(v2.position.y, frameBuffer->GetHeight());
            //
            v0.position.z = 1.0f / v0.position.z;
            v1.position.z = 1.0f / v1.position.z;
            v2.position.z = 1.0f / v2.position.z;

            // wireframe mode
            if (this->HasRenderFlags(RF_WIREFRAME_MODE)) {
                this->DrawLine(v0.position.ToVec2(), v1.position.ToVec2(), COLOR_WHITE, 0.0f);
                this->DrawLine(v1.position.ToVec2(), v2.position.ToVec2(), COLOR_WHITE, 0.0f);
                this->DrawLine(v2.position.ToVec2(), v0.position.ToVec2(), COLOR_WHITE, 0.0f);

                continue;
            }

            // reference : https://www.scratchapixel.com/lessons/3d-basic-rendering/rasterization-practical-implementation/overview-rasterization-algorithm.html
            int xMin = max(min(v0.position.x, min(v1.position.x, v2.position.x)), 0);
            int xMax = min(max(v0.position.x, max(v1.position.x, v2.position.x)), frameBuffer->GetWidth() - 1);
            int yMin = max(min(v0.position.y, min(v1.position.y, v2.position.y)), 0);
            int yMax = min(max(v0.position.y, max(v1.position.y, v2.position.y)), frameBuffer->GetHeight() - 1);

            float area = Math::EdgeFunction(v0.position.ToVec3(), v1.position.ToVec3(), v2.position.ToVec3());
            unsigned short mask = 0;

            for (int x = xMin; x < xMax; x++) {
                for (int y = yMin; y < yMax; y++) {
                    Vec3 pos(x + 0.5f, y + 0.5f, 1.0f);
                    float w0 = .0f, w1 = .0f, w2 = .0f;

                    if (GetAntiAliasingType() == AA_DEFAULT) {
                        if (!Math::PointInsideTriangle(v0.position.ToVec3(), v1.position.ToVec3(), v2.position.ToVec3(), pos, w0, w1, w2)) {
                            continue;
                        }

                        // top-left 约定
                        if (w0 == 0.0 && !Math::IsTopLeft(Vec2(v2.position.x - v1.position.x, v2.position.y - v1.position.y))) {
                            continue;
                        }

                        if (w1 == 0.0 && !Math::IsTopLeft(Vec2(v0.position.x - v2.position.x, v0.position.y - v2.position.y))) {
                            continue;
                        }

                        if (w2 == 0.0 && !Math::IsTopLeft(Vec2(v1.position.x - v0.position.x, v1.position.y - v0.position.y))) {
                            continue;
                        }
                    }
                    else if (GetAntiAliasingType() == AA_MSAA) {
                        //GetMSAAMask((msaaLevel_t)GetAntiAliasingLevel(), mask, v0.position.ToVec3(), v1.position.ToVec3(), v2.position.ToVec3(), pos);

                        //if (mask == 0) {
                        //    continue;
                        //}

                        //Math::PointInsideTriangle(v0.position.ToVec3(), v1.position.ToVec3(), v2.position.ToVec3(), pos, w0, w1, w2);
                    }

                    w0 /= area;
                    w1 /= area;
                    w2 /= area;

                    float denom = (w0 * v0.position.z + w1 * v1.position.z + w2 * v2.position.z);
                    pos.z = 1.0f / denom;
                    if (isnan(pos.z)) {
                        continue;
                    }

                    // Near/Far Plane Clpping
                    if (pos.z < 0.0f || pos.z > 0.999f) {
                        continue;
                    }

                    if (pos.x < 0 || pos.x >= frameBuffer->GetWidth() || y < 0 || y >= frameBuffer->GetHeight()) {
                        continue;
                    }

                    // reference : https://www.scratchapixel.com/lessons/3d-basic-rendering/rasterization-practical-implementation/perspective-correct-interpolation-vertex-attributes.html
                    Vec3 barycentric = Vec3(w0 * v0.position.z, w1 * v1.position.z, w2 * v2.position.z) * (1.0f / (w0 * v0.position.z + w1 * v1.position.z + w2 * v2.position.z));

                    v2f_t v;

                    v.position = Vec4(pos.x, pos.y, pos.z, 0.0f);
                    v.fragPos = Mat3(v0.fragPos.x, v1.fragPos.x, v2.fragPos.x, v0.fragPos.y, v1.fragPos.y, v2.fragPos.y, v0.fragPos.z, v1.fragPos.z, v2.fragPos.z) * barycentric;
                    v.normal = Mat3(v0.normal.x, v1.normal.x, v2.normal.x, v0.normal.y, v1.normal.y, v2.normal.y, v0.normal.z, v1.normal.z, v2.normal.z) * barycentric;
                    v.t_normal = Mat3(v0.t_normal.x, v1.t_normal.x, v2.t_normal.x, v0.t_normal.y, v1.t_normal.y, v2.t_normal.y, v0.t_normal.z, v1.t_normal.z, v2.t_normal.z) * barycentric;
                    v.texcoord = Vec2(Vec3(v0.texcoord.x, v1.texcoord.x, v2.texcoord.x).Dot(barycentric), Vec3(v0.texcoord.y, v1.texcoord.y, v2.texcoord.y).Dot(barycentric));
                    v.tangent = v0.tangent;
                    v.bitangent = v0.bitangent;
                    v.color = Mat4(v0.color.x, v1.color.x, v2.color.x, .0f,
                        v0.color.y, v1.color.y, v2.color.y, .0f,
                        v0.color.z, v1.color.z, v2.color.z, .0f,
                        v0.color.w, v1.color.w, v2.color.w, .0f) * Vec4(barycentric, 0.0f);

                    if (GetAntiAliasingType() == AA_DEFAULT) {
                        // Depth Test
                        int depthPos = y * frameBuffer->GetWidth() + x;
                        if (material->HasRenderFlags(RF_DEPTH_TEST) && frameBuffer->GetDepthBuffer()[depthPos] <= v.position.z) {
                            continue;
                        }

                        if (!material->HasRenderFlags(RF_TRANSPARENT)) {
                            frameBuffer->GetDepthBuffer()[depthPos] = v.position.z;
                        }

                        // Fragment Shading
                        Vec4 color = shader->Fragment(v);

                        byte *colorBuffer = frameBuffer->GetColorBuffer();
                        int colorPos = (frameBuffer->GetWidth() * y + x) * 3;

                        colorBuffer[colorPos + 0] = Float2ByteColor(color.x);
                        colorBuffer[colorPos + 1] = Float2ByteColor(color.y);
                        colorBuffer[colorPos + 2] = Float2ByteColor(color.z);
                    }
                }
            }
        }
    }
}
