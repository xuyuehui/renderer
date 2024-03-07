#ifndef __GRAPHIC_H__
#define __GRAPHIC_H__

#include "../math/vector.h"
#include "../utility/utility.h"
#include "../math/matrix.h"

namespace CG {
    typedef enum rendererAPIType {
        RAPI_SOFT, 
        RAPI_OPENGL, 
        RAPI_DIRECTX, 
        RAPI_VULKAN
    } rendererAPIType_t;

    class Window;
    class RenderWorld;
    class ShaderManager;
    class ModelManager;
    class TextureManager;
    typedef struct modelSurface_s modelSurface_t;

    typedef enum {
        AA_DEFAULT,
        AA_MSAA,
        AA_MAX,
    } antiAliasingType_t;

    typedef struct drawSurfaceContext_s {
        Mat4 model;
        Mat4 view;
        Mat4 proj;
    } drawSurfaceContext_t;

    class Renderer {
    public:
        virtual ~Renderer() {}

        virtual void Init(Window *window) = 0;
        virtual void ClearColorBuffer(const rgb &color) = 0;
        virtual void SwapBuffer() = 0;

        virtual void DrawLine(const Vec2 &s, const Vec2 &e, const color_t &color, float depth) = 0;
        virtual void DrawText(const char *text, const Vec2& pos, float size, const color_t &color, float gap) = 0;
        virtual void DrawSurface(const modelSurface_t *surface, const drawSurfaceContext_t &context) = 0;

        virtual RenderWorld *CreateRenderWorld() = 0;

        virtual ShaderManager *GetShaderManager() const = 0;
        virtual ModelManager *GetModelManager() const = 0;
        virtual TextureManager *GetTextureManager() const = 0;

        virtual void SetRenderFlags(uint32 f) = 0;
        virtual void UnsetRenderFlags(uint32 f)  = 0;
        virtual bool HasRenderFlags(uint32 f) const = 0;

        virtual void SetAntiAliasingType(antiAliasingType_t aa, uint32 opt) = 0;
        virtual antiAliasingType_t GetAntiAliasingType() const = 0;
        virtual uint32 GetAntiAliasingLevel() const = 0;
    };

    Renderer * CreateRendererAPI(rendererAPIType_t type);
}

#endif
