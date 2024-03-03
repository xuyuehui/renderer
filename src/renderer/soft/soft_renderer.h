#ifndef __SOFT_RENDERER__
#define __SOFT_RENDERER__

#include "../renderer.h"
#include "../../utility/utility.h"
#include "../../math/matrix.h"

namespace CG {
    class FrameBuffer;
    class ShaderManager;
    class Shader;
    class TextureManager;
    class ModelManager;
    class Material;

    struct modelSurface_s;
    typedef modelSurface_s modelSurface_t;

    class SoftRenderer : public Renderer {
    public:
        typedef struct drawSurfaceContext_s {
            Mat4 model;
            Mat4 view;
            Mat4 proj;
        } drawSurfaceContext_t;

    public:
        SoftRenderer();
        ~SoftRenderer();

        void Init(Window *window);
        void ClearColorBuffer(const rgb &color);
        void ClearDepthBuffer(float depth);
        void SwapBuffer();

        bool GetColorBufferDesc(renderTargetDesc_t &rtd);

        void DrawLine(const Vec2 &s, const Vec2 &e, const color_t &color, float depth);
        void DrawText(const char *text, const Vec2 &pos, float size, const color_t &color, float gap);

        RenderWorld *CreateRenderWorld();

        ShaderManager *GetShaderManager() const;
        ModelManager *GetModelManager() const;
        TextureManager *GetTextureManager() const;

        void SetRenderFlags(uint32 f);
        void UnsetRenderFlags(uint32 f);
        bool HasRenderFlags(uint32 f) const;

        void SetAntiAliasingType(antiAliasingType_t aa, uint32 opt);
        antiAliasingType_t GetAntiAliasingType() const;
        uint32 GetAntiAliasingLevel() const;
    public:
        // 已经准备好的缓冲区
        FrameBuffer *GetFrontFrameBuffer() const;

        // 当前帧正在更改的缓冲区
        FrameBuffer *GetBackFrameBuffer() const;

        void DrawSurface(const modelSurface_t *surface, const drawSurfaceContext_t &context);
    private:
        FrameBuffer *buffers[2];
        byte bufferIndex;
        bool needUpdated;

        Window *window;

        ShaderManager *shaderManager;
        ModelManager *modelManager;
        TextureManager *textureManager;

        Material *defaultMat;
        Shader *defaultShader;

        uint32 renderFlags;
        uint32 antiAlias;
    };

    inline FrameBuffer *SoftRenderer::GetFrontFrameBuffer() const {
        return buffers[bufferIndex];
    }

    inline FrameBuffer *SoftRenderer::GetBackFrameBuffer() const {
        return buffers[bufferIndex ^ 1];
    }

    inline ShaderManager *SoftRenderer::GetShaderManager() const {
        return shaderManager;
    }

    inline ModelManager *SoftRenderer::GetModelManager() const {
        return modelManager;
    }

    inline TextureManager *SoftRenderer::GetTextureManager() const {
        return textureManager;
    }

    inline void SoftRenderer::SetRenderFlags(uint32 f) {
        renderFlags |= f;
    }

    inline void SoftRenderer::UnsetRenderFlags(uint32 f) {
        renderFlags &= ~(f);
    }

    inline bool SoftRenderer::HasRenderFlags(uint32 f) const {
        return (renderFlags & f) != 0;
    }

    inline void SoftRenderer::SetAntiAliasingType(antiAliasingType_t aa, uint32 opt) {
        antiAlias = (aa << 16) | opt;
    }

    inline antiAliasingType_t SoftRenderer::GetAntiAliasingType() const {
        return (antiAliasingType_t)((antiAlias >> 16) & 0x00000FFFF);
    }

    inline uint32 SoftRenderer::GetAntiAliasingLevel() const {
        return antiAlias & 0x00000FFFF;
    }
}

#endif