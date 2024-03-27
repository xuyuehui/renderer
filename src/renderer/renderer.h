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
class FrameBuffer;

typedef struct modelSurface_s modelSurface_t;

typedef enum {
    AA_DEFAULT,
    AA_MSAA,
    AA_MAX,
} antiAliasingType_t;

typedef enum {
    SHADING_BLINN,
    SHADING_PBRM,
    SHADING_PBRS,
}shadingMode_t;

class Renderer {
public:
    virtual ~Renderer() {}

    virtual void Init(Window *window) = 0;
    virtual void ClearColorBuffer(const Vec3 &color) = 0;
    virtual void ClearDepthBuffer(float depth) = 0;
    virtual void SwapBuffer() = 0;

    virtual void DrawLine(const Vec2 &s, const Vec2 &e, const color_t &color, float depth) = 0;
    virtual void DrawText(const char *text, const Vec2& pos, float size, const color_t &color, float gap) = 0;
    virtual void DrawSurface(const RenderWorld *renderWorld, const modelSurface_t *surface, bool shadowPass, FrameBuffer *dst) = 0;

    virtual RenderWorld *CreateRenderWorld(shadingMode_t mode) = 0;

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
