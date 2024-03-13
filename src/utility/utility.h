#ifndef __UTILITY_H__
#define __UTILITY_H__

#include "../shared.h"

namespace CG {

typedef struct color_s {
    byte r;
    byte g;
    byte b;

    color_s(byte r, byte g, byte b) {
        this->r = r;
        this->g = g;
        this->b = b;
    }
}color_t;

#define COLOR_WHITE color_t(255, 255, 255)

}

#endif