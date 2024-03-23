#ifndef __RENDERWORLD_Base_H__
#define __RENDERWORLD_Base_H__

#include "renderworld.h"
#include "../utility/list.h"

namespace CG {

class RenderEntity;
class RenderLight;
class Renderer;
typedef struct modelSurface_s modelSurface_t;

class RenderWorld_Base : public RenderWorld {
public:
    typedef struct drawSurface_s {
        RenderEntity *entity;
        const modelSurface_t *surface;
        float distance;
    }drawSurface_t;

    RenderWorld_Base();
    virtual ~RenderWorld_Base();

    virtual uint32_t AddEntityDef(const renderEntity_t &entity);
    virtual uint32_t AddLightDef(const renderLight_t &light);
    virtual uint32_t AddSkyboxDef(const renderSkybox_t &skybox);

    virtual void SetRenderView(const renderView_t &renderView);
protected:
    List<RenderEntity *> entities;
    List<RenderLight *> lights;

    List<drawSurface_t> drawSurfaces;

    renderView_t primaryRenderView;
};

}

#endif
