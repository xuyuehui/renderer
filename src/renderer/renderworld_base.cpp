#include "renderworld_base.h"
#include "render_entity.h"
#include "render_light.h"

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
        return lights.Append(rl);
    }

    void RenderWorld_Base::SetRenderView(const renderView_t &renderView) {
        primaryRenderView = renderView;
    }
}