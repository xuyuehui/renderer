#ifndef __MATRIX_H__
#define __MATRIX_H__

#include "vector.h"
#include "../shared.h"

#define MATRIX_INVERSE_EPSILON		1e-14
#define MATRIX_EPSILON				1e-6

namespace CG {
    class Mat2 {
    private:

    };

    class Mat3 {
    public:
        Mat3();
        explicit Mat3(const Vec3 &x, const Vec3 &y, const Vec3 &z);
        explicit Mat3(float xx, float xy, float xz, float yx, float yy, float yz, float zx, float zy, float zz);

        Vec3 operator* (const Vec3 &vec) const;
    private:
        Vec3 mat[3];
    };

    inline Mat3::Mat3() {
    }

    inline Mat3::Mat3(const Vec3 &x, const Vec3 &y, const Vec3 &z) {
        mat[0] = x;
        mat[1] = y;
        mat[2] = z;
    }

    inline Mat3::Mat3(float xx, float xy, float xz, float yx, float yy, float yz, float zx, float zy, float zz) {
        mat[0][0] = xx; mat[0][1] = xy; mat[0][2] = xz;
        mat[1][0] = yx; mat[1][1] = yy; mat[1][2] = yz;
        mat[2][0] = zx; mat[2][1] = zy; mat[2][2] = zz;
    }

    inline Vec3 Mat3::operator* (const Vec3 &vec) const {
        return Vec3(
            mat[0].x * vec.x + mat[1].x * vec.y + mat[2].x + vec.z,
            mat[0].y * vec.x + mat[1].y * vec.y + mat[2].y + vec.z,
            mat[0].z * vec.x + mat[1].z * vec.y + mat[2].z + vec.z
        );
    }

    class Mat4 {
    public:
        Mat4();
        explicit Mat4(const Vec4 &x, const Vec4 &y, const Vec4 &z, const Vec4 &w);
        explicit Mat4(float xx, float xy, float xz, float xw,
            float yx, float yy, float yz, float yw,
            float zx, float zy, float zz, float zw,
            float wx, float wy, float wz, float ww);

        const Vec4 &operator[](int index) const;
        Vec4 &operator[](int index);

        Mat4 operator*(const Mat4 &a) const;
        Vec4 operator*(const Vec4 &vec) const;
        Vec3 operator*(const Vec3 &vec) const;

        Mat4 Transport() const;
        Mat4 &TransportSelf();
        Mat4 Inverse() const;
        bool InverseSelf();
    private:
        Vec4 mat[4];
    };

    inline Mat4::Mat4() {
    }

    inline Mat4::Mat4(const Vec4 &x, const Vec4 &y, const Vec4 &z, const Vec4 &w) {
        mat[0] = x;
        mat[1] = y;
        mat[2] = z;
        mat[3] = w;
    }

    inline Mat4::Mat4(float xx, float xy, float xz, float xw,
        float yx, float yy, float yz, float yw,
        float zx, float zy, float zz, float zw,
        float wx, float wy, float wz, float ww) {
        mat[0][0] = xx; mat[0][1] = xy; mat[0][2] = xz; mat[0][3] = xw;
        mat[1][0] = yx; mat[1][1] = yy; mat[1][2] = yz; mat[1][3] = yw;
        mat[2][0] = zx; mat[2][1] = zy; mat[2][2] = zz; mat[2][3] = zw;
        mat[3][0] = wx; mat[3][1] = wy; mat[3][2] = wz; mat[3][3] = ww;
    }

    inline const Vec4 &Mat4::operator[](int index) const {
        assert(index >= 0 && index < 4);
        return mat[index];
    }

    inline Vec4 &Mat4::operator[](int index) {
        assert(index >= 0 && index < 4);
        return mat[index];
    }

    inline Mat4 Mat4::operator*(const Mat4 &a) const {
        Mat4 dst;
        const float *m1Ptr = reinterpret_cast<const float *>(this);
        const float *m2Ptr = reinterpret_cast<const float *>(&a);
        float *dstPtr = reinterpret_cast<float *>(&dst);

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                *dstPtr = m1Ptr[0] * m2Ptr[0 * 4 + j]
                    + m1Ptr[1] * m2Ptr[1 * 4 + j]
                    + m1Ptr[2] * m2Ptr[2 * 4 + j]
                    + m1Ptr[3] * m2Ptr[3 * 4 + j];

                dstPtr++;
            }

            m1Ptr += 4;
        }

        return dst;
    }

    inline Vec4 Mat4::operator*(const Vec4 &vec) const {
        return Vec4(
                mat[0].x * vec.x + mat[0].y * vec.y + mat[0].z * vec.z + mat[0].w * vec.w,
                mat[1].x * vec.x + mat[1].y * vec.y + mat[1].z * vec.z + mat[1].w * vec.w,
                mat[2].x * vec.x + mat[2].y * vec.y + mat[2].z * vec.z + mat[2].w * vec.w,
                mat[3].x * vec.x + mat[3].y * vec.y + mat[3].z * vec.z + mat[3].w * vec.w
       );
    }

    inline Vec3 Mat4::operator*(const Vec3 &vec) const {
        float s = mat[3].x * vec.x + mat[3].y * vec.y + mat[3].z * vec.z + mat[3].w;
        if (s == 0.0f) {
            return Vec3(0.0f, 0.0f, 0.0f);
        }

        if (s == 1.0f) {
            return Vec3(
                mat[0].x * vec.x + mat[0].y * vec.y + mat[0].z * vec.z + mat[0].w,
                mat[1].x * vec.x + mat[1].y * vec.y + mat[1].z * vec.z + mat[1].w,
                mat[2].x * vec.x + mat[2].y * vec.y + mat[2].z * vec.z + mat[2].w
            );
        }
        else {
            float invS = 1.0f / s;
            return Vec3(
                (mat[0].x * vec.x + mat[0].y * vec.y + mat[0].z * vec.z + mat[0].w) * invS,
                (mat[1].x * vec.x + mat[1].y * vec.y + mat[1].z * vec.z + mat[1].w) * invS,
                (mat[2].x * vec.x + mat[2].y * vec.y + mat[2].z * vec.z + mat[2].w) * invS
            );
        }
    }

    inline Mat4 Mat4::Inverse() const {
        Mat4 invMat;

        invMat = *this;
        bool r = invMat.InverseSelf();
        assert(r);

        return invMat;
    }
}

#endif