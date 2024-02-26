#include "msaa.h"
#include "../../utility/list.h"
#include "../../math/math.h"

namespace CG {
    /* MSAA Specs */

    /*  2X MSAA pattern
    *    ------------
    *   |    +       |        (-0.1,  0.4) for case0
    *   |         -  |        ( 0.4,  0.1) for case1
    *   |  -         |        (-0.4, -0.1) for case1
    *   |       +    |        ( 0.1, -0.4) for case0
    *    ------------
    */

    /*  4X MSAA pattern
    *    ------------
    *   |    *       |        (-0.1,  0.4)
    *   |         *  |        ( 0.4,  0.1)
    *   |  *         |        (-0.4, -0.1)
    *   |       *    |        ( 0.1, -0.4)
    *    ------------
    */

    /*  8X MSAA pattern
    *    ------------
    *   | *     *    |        (-0.375,  0.375) (0.125,  0.375)
    *   |    *     * |        (-0.125,  0.125) (0.375,  0.125)
    *   | *     *    |        (-0.375, -0.125) (0.125, -0.125)
    *   |    *     * |        (-0.125, -0.375) (0.375, -0.375)
    *    ------------
    */

    const float MSAA2X_PATTERN[][2] = {
        {-0.1f,  0.4f},
        { 0.1f, -0.4f},
    };

    const float MSAA4X_PATTERN[][2] = {
        {-0.1f,  0.4f},
        { 0.4f,  0.1f},
        {-0.4f, -0.1f},
        { 0.1f, -0.4f},
    };

    const float MSAA8X_PATTERN[][2] = {
        {-0.375f,  0.375f}, {0.125f,  0.375f},
        {-0.125f,  0.125f}, {0.375f,  0.125f},
        {-0.375f, -0.125f}, {0.125f, -0.125f},
        {-0.125f, -0.375f}, {0.375f,  0.375f},
    };

    void GetMSAAMask(msaaLevel_t sampleType, unsigned short &mask, const Vec3 &v0, const Vec3 &v1, const Vec3 &v2, const Vec3 &pos) {
        mask = 0;
        float tp0, tp1, tp2;
        int sampleNum = 0;

        Vec3 samples[8];

        switch (sampleType) {
        case MSAA_LEVEL_2X:
            sampleNum = 2;
            break;
        case MSAA_LEVEL_4X:
            sampleNum = 4;
            break;
        case MSAA_LEVEL_8X:
            sampleNum = 8;
            break;
        }

        for (int i = 0; i < sampleNum; i++) {
            samples[i] = Vec3(pos.x + MSAA2X_PATTERN[i][0], pos.y + MSAA2X_PATTERN[i][1], 1.0f);
        }

        for (int i = 0; i < sampleNum; i++) {
            if (!Math::PointInsideTriangle(v0, v1, v2, samples[i], tp0, tp1, tp2)) {
                mask |= (1 << i);
            }
        }
    }
}