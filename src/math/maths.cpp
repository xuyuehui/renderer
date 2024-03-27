#include "math.h"
#include "../shared.h"
#include "vector.h"
#include "matrix.h"
#include "quat.h"
#include "angles.h"

namespace CG {

bool Math::initialized = false;
dword Math::iSqrt[SQRT_TABLE_SIZE];

const float Math::PI            = 3.14159265358979323846264338327950288f;
const float Math::TWO_PI        = 2.0f * PI;
const float Math::HALF_PI       = 0.5f * PI;
const float Math::ONEFOURTH_PI  = 0.25f * PI;
const float Math::E             = 2.71828182845904523536f;
const float Math::M_DEG2RAD     = PI / 180.0f;
const float Math::M_RAD2DEG     = 180.0f / PI;
const float Math::EPSILON       = 1.192092896e-07f;


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

Mat4 Math::FromTRS(const Vec3 &t, const Quat &r, const Vec3 &s) {
    Mat4 translation = Mat4::FromTranslate(t.x, t.y, t.z);
    Mat4 rotation = Mat4::FromQuat(r);
    Mat4 scale = Mat4::FromScale(s.x, s.y, s.z);
    return translation * rotation * scale;
}

// Fundamentals of Computer Graphics, Fourth Edition, Chapter 7.3 | GAMES101 Lecture 05
Mat4 Math::Perspective(float fovY, float aspect, float near, float far) {
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
Mat4 Math::LookAt(const Vec3 &eye, const Vec3 &target, const Vec3 &up) {
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

/*
 * right: the coordinates for the right clipping planes (left == -right)
 * top: the coordinates for the top clipping planes (bottom == -top)
 * near, far: the distances to the near and far depth clipping planes
 *
 * 1/r    0         0             0
 *   0  1/t         0             0
 *   0    0  -2/(f-n)  -(f+n)/(f-n)
 *   0    0         0             1
 *
 * this is the same as
 *     float left = -right;
 *     float bottom = -top;
 *     mat4_ortho(left, right, bottom, top, near, far);
 *
 * see http://www.songho.ca/opengl/gl_projectionmatrix.html
 */
Mat4 Math::Orthographic(float right, float top, float near, float far) {
    float zRange = far - near;
    Mat4 mat;
    mat.Indentity();

    assert(right > 0 && top > 0 && zRange > 0);

    mat[0][0] = 1 / right;
    mat[1][1] = 1 / top;
    mat[2][2] = -2 / zRange;
    mat[2][3] = -(near + far) / zRange;

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

// Fundamentals of Computer Graphics, Fourth Edition, Chapter 8.7.4
Quat Math::MatrixToQuat(const Mat4 &mat) {
    Quat rotation;

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

    float mult = InvSqrt(fourSquareMinus[0] + 1.0f) * 0.5f;
    float biggestValue = 0.25f / mult;

    switch (biggestIndex) {
    case 0:
        rotation.w = biggestValue;
        rotation.x = (mat[2][1] - mat[1][2]) * mult;
        rotation.y = (mat[0][2] - mat[2][0]) * mult;
        rotation.z = (mat[1][0] - mat[0][1]) * mult;
        break;
    case 1:
        rotation.x = biggestValue;
        rotation.w = (mat[2][1] - mat[1][2]) * mult;
        rotation.y = (mat[1][0] + mat[0][1]) * mult;
        rotation.z = (mat[0][2] + mat[2][0]) * mult;
        break;
    case 2:
        rotation.y = biggestValue;
        rotation.w = (mat[0][2] - mat[2][0]) * mult;
        rotation.x = (mat[1][0] + mat[0][1]) * mult;
        rotation.z = (mat[2][1] + mat[1][2]) * mult;
        break;
    case 3:
        rotation.z = biggestValue;
        rotation.w = (mat[1][0] - mat[0][1]) * mult;
        rotation.x = (mat[0][2] + mat[2][0]) * mult;
        rotation.y = (mat[2][1] + mat[1][2]) * mult;
        break;
    }

    return rotation;
}

Vec3 Math::MatrixToTranslate(const Mat4 &mat) {
    Vec3 translate;

    translate.x = mat[0][3];
    translate.y = mat[1][3];
    translate.z = mat[2][3];

    return translate;
}

Vec4 Math::Saturate(const Vec4 &v) {
    return Vec4(
        Saturate(v.x),
        Saturate(v.y),
        Saturate(v.z),
        Saturate(v.w)
    );
}

}
