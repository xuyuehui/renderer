#include "renderworld_base.h"
#include "render_entity.h"
#include "render_light.h"
#include "../utility/str.h"
#include "../math/matrix.h"

namespace CG {

RenderWorld_Base::RenderWorld_Base() {
}

RenderWorld_Base::~RenderWorld_Base() {
}

uint32_t RenderWorld_Base::AddEntityDef(const renderEntity_t &entity) {
    RenderEntity *re = new RenderEntity();
    re->UpdateRenderParams(&entity);
    return entities.Append(re);
}

uint32_t RenderWorld_Base::AddLightDef(const renderLight_t &light) {
    RenderLight *rl = new RenderLight();
    rl->UpdateRenderParams(&light);
    return lights.Append(rl);
}

uint32_t RenderWorld_Base::AddSkyboxDef(const renderSkybox_t &skybox) {
    return 0;
}

void RenderWorld_Base::SetRenderView(const renderView_t &renderView) {
    primaryRenderView = renderView;
}

}