#include "math.h"
#include "../shared.h"
#include "vector.h"
#include "matrix.h"
#include "quat.h"
#include "angles.h"

namespace CG {
    bool Math::initialized = false;
    dword Math::iSqrt[SQRT_TABLE_SIZE];

    void Math::Init() {
        union _flint fi, fo;

        for (int i = 0; i < SQRT_TABLE_SIZE; i++) {
            fi.i = ((EXP_BIAS - 1) << EXP_POS) | (i << LOOKUP_POS);
            fo.f = (float)(1.0 / sqrt(fi.f));
            iSqrt[i] = ((dword)(((fo.i + (1 << (SEED_POS - 2))) >> SEED_POS) & 0XFF)) << SEED_POS;
        }

        iSqrt[SQRT_TABLE_SIZE / 2] = ((dword)(0xFF)) << (SEED_POS);
        initialized = true;
    }

    // reference : 3D Math Primer for Graphics and Game Development, Second Edition, Chapter 8.7.3
    Mat4 Math::FromRTS(const Vec3 &translate, const Quat &rotation, const Vec3 &scale) {
        float xx = rotation.x * rotation.x;
        float xy = rotation.x * rotation.y;
        float xz = rotation.x * rotation.z;
        float xw = rotation.x * rotation.w;
        float yy = rotation.y * rotation.y;
        float yz = rotation.y * rotation.z;
        float yw = rotation.y * rotation.w;
        float zz = rotation.z * rotation.z;
        float zw = rotation.z * rotation.w;

        Mat4 mat(
            (1.0f - 2.0f * yy - 2.0f * zz) * scale.x,	2.0f * xy - 2.0f * zw,						2.0f * xz + 2.0f * yw,						translate.x,
            2.0f * xy + 2.0f * zw,						(1.0f - 2.0f * xx - 2.0f * zz) * scale.y,	2.0f * yz - 2.0f * xw,						translate.y,
            2.0f * xz - 2.0f * yw,						2.0f * yz + 2.0f * xw,						(1.0f - 2.0f * xx - 2.0f * yy) * scale.z,	translate.z,
            0.0f,										0.0f,										0.0f,										1.0f
        );

        return mat;
    }

    // reference : Fundamentals of Computer Graphics, Fourth Edition, Chapter 7.3 | GAMES101 Lecture 05
    Mat4 Math::ProjectMatrix(float fovY, float aspect, float near, float far) {
        float tanFovYHalf = tanf(fovY / 2.0f);
        Mat4 mat(
            1.0f / (aspect * tanFovYHalf),	0.0f,					0.0f,						0.0f,
            0.0f,							1.0f / tanFovYHalf,		0.0f,						0.0f,
            0.0f,							0.0f,					-(near + far) / (far-near),	-2.0f * far * near / (far-near),
            0.0f,							0.0f,					-1.0f,						0.0f
        );
        return mat;
    }

    // reference : Fundamentals of Computer Graphics, Fourth Edition, Chapter 7.1.3 | GAMES101 Lecture 04
    Mat4 Math::ViewMatrix(const Vec3 &eye, const Vec3 &target, const Vec3 &up) {
        Vec3 forward = (eye - target).Normalized();
        Vec3 left = up.Cross(forward).Normalized();
        Vec3 upward = forward.Cross(left);

        Mat4 mat(
            left.x,		left.y,		left.z,		-(left.x * eye.x + left.y * eye.y + left.z * eye.z),
            upward.x,	upward.y,	upward.z,	-(upward.x * eye.x + upward.y * eye.y + upward.z * eye.z),
            forward.x,	forward.y,	forward.z,	-(forward.x * eye.x + forward.y * eye.y + forward.z * eye.z),
            0.0f,		0.0f,		0.0f,		1.0f
        );
        return mat;
    }

    // reference : https://www.scratchapixel.com/lessons/3d-basic-rendering/rasterization-practical-implementation/rasterization-stage.html
    float Math::EdgeFunction(const Vec3 &a, const Vec3 &b, const Vec3 &c) {
        return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
    }

    bool Math::PointInsideTriangle(const Vec3 &v0, const Vec3 &v1, const Vec3 &v2, const Vec3 &pos, float &w0, float &w1, float &w2) {
        w0 = EdgeFunction(v1, v2, pos);
        w1 = EdgeFunction(v2, v0, pos);
        w2 = EdgeFunction(v0, v1, pos);

        return (w0 >= 0 && w1 >= 0 && w2 >= 0) || (w0 < 0 && w1 < 0 && w2 < 0);
    }

    bool Math::IsTopLeft(const Vec2 &v) {
        return (v.y >= 0 && fabs(v.y) < EPSILON && v.x > 0) || v.y > 0;
    }

    void Math::PerspectiveDivision(Vec4 &v) {
        v.w = 1.0f / v.w;
        v.x *= v.w;
        v.y *= v.w;
        v.z *= v.w;
    }

}
