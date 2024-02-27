#ifndef __RASTERIZER_H__
#define __RASTERIZER_H__

#include "../../shared.h"
#include "../../utility/utility.h"

namespace CG {
    class FrameBuffer;
    class Vec2;
    class Vec3;

    void R_DrawPixel(const FrameBuffer &frameBuffer, int x, int y, const color_t &color, float depth);
    void R_DrawLine(const FrameBuffer &frameBuffer, const Vec2 &v1, const Vec2 &v2, const color_t &color, float depth);
}

#endif