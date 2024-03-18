#ifndef __RENDERWORLD_H__
#define __RENDERWORLD_H__

#include "../shared.h"
#include "../math/vector.h"
#include "../math/quat.h"
#include "../math/matrix.h"
#include "../utility/utility.h"

namespace CG {

class RenderModel;
class IProgram;

typedef struct renderEntity_s {
    RenderModel *model;

    Vec3 position;
    Quat rotation;
    Vec3 scale;

    Mat4 orginal;
} renderEntity_t;

typedef struct renderLight_s {
    Vec3 position;
    Quat rotation;

    Vec3 background;
    float ambient;
    float punctual;
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

typedef struct renderSkybox_s {
    RenderModel *model;
}renderSkybox_t;

class RenderWorld {
public:
    virtual ~RenderWorld() {}

    virtual uint32_t AddEntityDef(const renderEntity_t &entity) = 0;
    virtual uint32_t AddLightDef(const renderLight_t &light) = 0;
    virtual uint32_t AddSkyboxDef(const renderSkybox_t &skybox) = 0;

    virtual void SetRenderView(const renderView_t &renderView) = 0;
    virtual void RenderScene() = 0;

    virtual IProgram *GetProgram() const = 0;
};

}

#endif
