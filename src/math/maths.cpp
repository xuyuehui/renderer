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

    // 3D Math Primer for Graphics and Game Development, Second Edition, Chapter 8.7.3
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

    // Fundamentals of Computer Graphics, Fourth Edition, Chapter 7.3 | GAMES101 Lecture 05
    Mat4 Math::ProjectMatrix(float fovY, float aspect, float near, float far) {
        float tanFovYHalf = tanf(fovY / 2.0f);
        Mat4 mat(
            1.0f / (aspect * tanFovYHalf),	0.0f,					0.0f,						    0.0f,
            0.0f,							1.0f / tanFovYHalf,		0.0f,						    0.0f,
            0.0f,							0.0f,					-(near + far) / (far - near),	-2.0f * far * near / (far - near),
            0.0f,							0.0f,					-1.0f,						    0.0f
        );
        return mat;
    }

    // Fundamentals of Computer Graphics, Fourth Edition, Chapter 7.1.3 | GAMES101 Lecture 04
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

    // https://www.scratchapixel.com/lessons/3d-basic-rendering/rasterization-practical-implementation/rasterization-stage.html
    float Math::EdgeFunction(const Vec3 &a, const Vec3 &b, const Vec3 &c) {
        return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
    }

    bool Math::PointInsideTriangle(const Vec3 &v0, const Vec3 &v1, const Vec3 &v2, const Vec3 &pos, float &w0, float &w1, float &w2) {
        w0 = EdgeFunction(v1, v2, pos);
        w1 = EdgeFunction(v2, v0, pos);
        w2 = EdgeFunction(v0, v1, pos);

        return (w0 >= 0 && w1 >= 0 && w2 >= 0) || (w0 <= 0 && w1 <= 0 && w2 <= 0);
    }

    bool Math::IsTopLeft(const Vec2 &v) {
        return (v.y >= 0 && fabs(v.y) < EPSILON && v.x > 0) || v.y > 0;
    }

    // Fundamentals of Computer Graphics, Fourth Edition, Chapter 8.7.3 
    void Math::MatrixToQuat(const Mat4 &mat, Quat &rotation) {
        float fourSquareMinus[] = {
            mat[0][0] + mat[1][1] + mat[2][2],      // w
            mat[0][0] - mat[1][1] - mat[2][2],      // x
            mat[1][1] - mat[0][0] - mat[2][2],      // y
            mat[2][2] - mat[0][0] - mat[1][1]       // z
        };

        int biggestIndex = 0;
        for (int i = 1; i < 4; i++) {
            if (fourSquareMinus[i] > fourSquareMinus[0]) {
                fourSquareMinus[0] = fourSquareMinus[i];
                biggestIndex = i;
            }
        }

        float mult = InvSqrt(fourSquareMinus[0] + 1.0) * 0.5f;
        float biggestValue = 0.25f / mult;
        float x, y, z, w;

        switch (biggestIndex) {
        case 0:
            w = biggestValue;
            x = (mat[1][2] - mat[2][1]) * mult;
            y = (mat[2][0] - mat[0][2]) * mult;
            z = (mat[0][1] - mat[1][0]) * mult;
            break;
        case 1:
            x = biggestValue;
            w = (mat[1][2] - mat[2][1]) * mult;
            y = (mat[0][1] + mat[1][0]) * mult;
            z = (mat[2][0] + mat[0][2]) * mult;
            break;
        case 2:
            y = biggestValue;
            w = (mat[2][0] - mat[0][2]) * mult;
            x = (mat[0][1] + mat[1][0]) * mult;
            z = (mat[1][2] + mat[2][1]) * mult;
            break;
        case 3:
            z = biggestValue;
            w = (mat[0][1] - mat[1][0]) * mult;
            x = (mat[2][0] + mat[0][2]) * mult;
            y = (mat[1][2] + mat[2][1]) * mult;
            break;
        }
    }

    void Math::MatrixToRTS(const Mat4 &mat, Vec3 &translate, Quat &rotation, Vec3 &scale) {
        MatrixToQuat(mat, rotation);

        translate.x = mat[0][3];
        translate.y = mat[1][3];
        translate.z = mat[2][3];

        // m00 = (1.0f - 2.0f * yy - 2.0f * zz) * scale.x
        // m11 = (1.0f - 2.0f * xx - 2.0f * zz) * scale.y
        // m22 = (1.0f - 2.0f * xx - 2.0f * yy) * scale.z
        float temp = 0;
        
        temp = (1.0f - 2.0f * rotation.y * rotation.y - 2.0f * rotation.z * rotation.z);
        scale.x = fabs(temp) > EPSILON ? (mat[0][0] / temp) : 0.0f;

        temp = (1.0f - 2.0f * rotation.x * rotation.x - 2.0f * rotation.z * rotation.z);
        scale.y = fabs(temp) > EPSILON ? (mat[1][1] / temp) : 0.0f;

        temp = (1.0f - 2.0f * rotation.x * rotation.x - 2.0f * rotation.y * rotation.y);
        scale.z = fabs(temp) > EPSILON ? (mat[2][2] / temp) : 0.0f;
    }
}
