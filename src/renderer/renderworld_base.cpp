#include "renderworld_base.h"

namespace CG {
    RenderWorld_Base::RenderWorld_Base() : renderFlags(0), antiAlias(AA_DEFAULT){
    }

    RenderWorld_Base::~RenderWorld_Base() {
    }

    uint32_t RenderWorld_Base::AddEntityDef(const renderEntity_t &entity) {
        return -1;
    }

    uint32_t RenderWorld_Base::AddLightDef(const renderLight_t &light) {
        return -1;
    }

    void RenderWorld_Base::SetRenderView(const renderView_t &renderView) {
        primaryRenderView = renderView;
    }
}