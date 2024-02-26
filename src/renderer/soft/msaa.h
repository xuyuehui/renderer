#ifndef __MSAA_H__
#define __MSAA_H__

#include "../../math/vector.h"
#include "../renderworld.h"

namespace CG {
    typedef enum msaaLevel {
        MSAA_LEVEL_2X,
        MSAA_LEVEL_4X,
        MSAA_LEVEL_8X,
    } msaaLevel_t;

    void GetMSAAMask(msaaLevel_t sampleType, unsigned short &mask, const Vec3 &v0, const Vec3 &v1, const Vec3 &v2, const Vec3 &pos);
}

#endif