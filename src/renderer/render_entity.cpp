#include "render_entity.h"

namespace CG {

RenderEntity::RenderEntity() {
}

void RenderEntity::UpdateRenderParams(const renderEntity_t *params) {
    if (!params) {
        return;
    }

    memcpy(&re, params, sizeof(re));

    transform = params->orginal * Math::FromTRS(re.position, re.rotation, re.scale);
}

}
