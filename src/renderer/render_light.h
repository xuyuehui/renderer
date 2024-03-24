#ifndef __RENDER_LIGHT_H__
#define __RENDER_LIGHT_H__

#include "renderworld.h"

namespace CG {

class RenderLight {
public:
    void UpdateRenderParams(const renderLight_t *params);
    const renderLight_t &RenderParams() const;
private:
    renderLight_t rl;
};

}

#endif