#include "render_light.h"

namespace CG {

void RenderLight::UpdateRenderParams(const renderLight_t *params) {
    if (!params) {
        return;
    }

    memcpy(&rl, params, sizeof(rl));
}

const renderLight_t &RenderLight::RenderParams() const {
    return rl;
}

}