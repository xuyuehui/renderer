#ifndef __RENDERWORLD_H__
#define __RENDERWORLD_H__

#include "../shared.h"
#include "../math/vector.h"
#include "../math/quat.h"
#include "../utility/utility.h"

namespace CG {
    class RenderModel;

    typedef struct renderEntity_s {
        RenderModel *model;

        Vec3 position;
        Quat rotation;
        Vec3 scale;
    } renderEntity_t;

    typedef struct renderLight_s {
        Vec3 position;
        Quat rotation;
    }renderLight_t;

    typedef struct renderView_s {
        Vec3 position;
        Vec3 target;
        Vec3 up;

        float fovY;
        float aspect;
        float near;
        float far;
    }renderView_t;

    typedef enum {
        AA_DEFAULT,
        AA_MSAA,
        AA_MAX,
    } antiAliasingType_t;

    class RenderWorld {
    public:
        virtual ~RenderWorld() {}

        virtual uint32_t AddEntityDef(const renderEntity_t &entity) = 0;
        virtual uint32_t AddLightDef(const renderLight_t &light) = 0;

        virtual void SetRenderView(const renderView_t &renderView) = 0;
        virtual void RenderScene() = 0;

        virtual void SetRenderFlags(uint32 f) = 0;
        virtual void UnsetRenderFlags(uint32 f) = 0;
        virtual bool HasRenderFlags(uint32 f) const = 0;

        virtual void SetAntiAliasingType(antiAliasingType_t aa, uint32 opt) = 0;
        virtual antiAliasingType_t GetAntiAliasingType() const = 0;
        virtual uint32 GetAntiAliasingLevel() const = 0;
    };
}

#endif
