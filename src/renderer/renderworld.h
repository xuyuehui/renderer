#ifndef __RENDERWORLD_H__
#define __RENDERWORLD_H__

#include "../shared.h"
#include "../math/vector.h"
#include "../math/quat.h"
#include "../utility/utility.h"

#define COLOR_WHITE color_t(255, 255, 255)

namespace CG {
    const uint32 RF_BACK_FACE_CULLING		= 0x00000001;
    const uint32 RF_WIREFRAME_MODE			= 0x00000002;

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
        SAMPLE_DEFAULT,
        SAMPLE_MSAA2X,
        SAMPLE_MSAA4X,
        SAMPLE_MSAA8X,
        SAMPLE_MAX,
    } sampleType_t;

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

        virtual void SetSampleType(sampleType_t st) = 0;
        virtual sampleType_t GetSampleType() const = 0;
    };
}

#endif