#ifndef __GRAPHIC_H__
#define __GRAPHIC_H__

#include "../math/vector.h"
#include "../utility/utility.h"

namespace CG {
    typedef enum rendererAPIType {
        RAPI_SOFT, 
        RAPI_OPENGL, 
        RAPI_DIRECTX, 
        RAPI_VULKAN
    } rendererAPIType_t;

    class Window;
    class RenderWorld;

    typedef struct renderTargetDesc_s {
        byte *data;
        int width;
        int height;
        int bitCount;
        byte tag;
    }renderTargetDesc_t;

    class Renderer {
    public:
        virtual ~Renderer() {}

        virtual void Init(Window *window) = 0;
        virtual void ClearColorBuffer(const rgb &color) = 0;
        virtual void SwapBuffer() = 0;

        virtual bool GetColorBufferDesc(renderTargetDesc_t &rtd) = 0;

        virtual void DrawLine(const Vec2 &s, const Vec2 &e, const color_t &color, float depth) = 0;
        virtual void DrawText(const char *text, const Vec2& pos, float size, const color_t &color, float gap) = 0;

        virtual uint32_t LoadShader(const char *name) = 0;

        virtual RenderWorld *CreateRenderWorld() = 0;
    };

    Renderer * CreateRendererAPI(rendererAPIType_t type);
}

#endif
