#ifndef __MATHS_H__
#define __MATHS_H__

#include "../shared.h"

namespace CG {

#define DEG2RAD(a)      ((a) * Math::M_DEG2RAD)
#define RAD2DEG(a)      ((a) * Math::M_RAD2DEG)

class Vec2;
class Vec3;
class Vec4;
class Mat4;
class Quat;

// https://github.com/id-Software/DOOM-3
class Math {
public:
    static void Init();
    static Mat4 FromTRS(const Vec3 &translate, const Quat &rotation, const Vec3 &scale);
    static Mat4 Perspective(float fovY, float aspect, float near, float far);
    static Mat4 LookAt(const Vec3 &eye, const Vec3 &target, const Vec3 &up);

    static Quat MatrixToQuat(const Mat4 &mat);
    static Vec3 MatrixToTranslate(const Mat4 &mat);

    static float InvSqrt(float sqrLength);
    static void SinCos(float a, float &s, float &c);

    static float EdgeFunction(const Vec3 &a, const Vec3 &b, const Vec3 &c);
    static bool PointInsideTriangle(const Vec3 &v0, const Vec3 &v1, const Vec3 &v2, const Vec3 &pos, float &w0, float &w1, float &w2);
    static bool IsTopLeft(const Vec2 &v);
private:
    enum {
        LOOKUP_BITS = 8,
        EXP_POS = 23,
        EXP_BIAS = 127,
        LOOKUP_POS = (EXP_POS-LOOKUP_BITS),
        SEED_POS = (EXP_POS-8),
        SQRT_TABLE_SIZE = (2<<LOOKUP_BITS),
        LOOKUP_MASK = (SQRT_TABLE_SIZE-1)
    };

    union _flint {
        dword i;
        float f;
    };

    static dword iSqrt[SQRT_TABLE_SIZE];
    static bool initialized;

public:
    static const float PI;
    static const float TWO_PI;
    static const float HALF_PI;
    static const float ONEFOURTH_PI;
    static const float E;
    static const float M_DEG2RAD;
    static const float M_RAD2DEG;
    static const float EPSILON;
};

inline float Math::InvSqrt(float x) {
    dword a = ((union _flint *)(&x))->i;
    union _flint seed;

    assert(initialized);

    double y = x * 0.5f;
    seed.i = ((((3 * EXP_BIAS - 1) - ((a >> EXP_POS) & 0XFF)) >> 1) << EXP_POS) | iSqrt[(a >> (EXP_POS - LOOKUP_BITS)) & LOOKUP_MASK];
    double r = seed.f;

    r = r * (1.5f - r * r * y);
    r = r * (1.5f - r * r * y);

    return (float)r;
}

inline void Math::SinCos(float a, float &s, float &c) {
    s = sinf(a);
    c = cosf(a);
}

}

#endif
