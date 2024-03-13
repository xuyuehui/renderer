#ifndef __RENDERWORLD_Base_H__
#define __RENDERWORLD_Base_H__

#include "renderworld.h"
#include "../utility/list.h"

namespace CG {

class RenderEntity;
class RenderLight;
class Renderer;

class RenderWorld_Base : public RenderWorld {
public:
    RenderWorld_Base();
    virtual ~RenderWorld_Base();

    virtual uint32_t AddEntityDef(const renderEntity_t &entity);
    virtual uint32_t AddLightDef(const renderLight_t &light);
    virtual uint32_t AddSkyboxDef(const renderSkybox_t &skybox);

    virtual void SetRenderView(const renderView_t &renderView);
protected:
    List<RenderEntity *> entities;
    List<RenderLight *> lights;

    renderView_t primaryRenderView;
};

}

#endif
