#include "angles.h"

namespace CG {

Quat Angles::ToQuat() const {
    float sx, cx, sy, cy, sz, cz;
    float sxcy, cxcy, sxsy, cxsy;

    Math::SinCos(DEG2RAD(yaw) * 0.5f, sy, cy);
    Math::SinCos(DEG2RAD(pitch) * 0.5f, sx, cx);
    Math::SinCos(DEG2RAD(roll) * 0.5f, sz, cz);

    sxcy = sx * cy;
    cxcy = cx * cy;
    sxsy = sx * sy;
    cxsy = cx * sy;
    
    return Quat(sxcy * sz + cxsy * sz, cxsy * cz - sxcy * sz, cxcy * sz - sxsy * cz, cxcy * cz + sxsy * sz);
}

}