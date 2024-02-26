#ifndef __VECTOR_H__
#define __VECTOR_H__

#include "../shared.h"
#include "math.h"

namespace CG {
    class Vec2 {
    public:
        float x;
        float y;

        Vec2();
        explicit Vec2(float x, float y);

        float operator[] (int index);
        Vec2 &operator= (const Vec2 &a);
    };

    inline Vec2::Vec2() : x(.0f), y(.0f){

    }

    inline Vec2::Vec2(float x, float y) {
        this->x = x;
        this->y = y;
    }

    inline float Vec2::operator[] (int index) {
        assert(index >= 0 && index < 2);
        return (&x)[index];
    }

    inline Vec2 &Vec2::operator= (const Vec2 &a) {
        x = a.x;
        y = a.y;

        return *this;
    }

    class Vec3 {
    public:
        float x;
        float y;
        float z;

        Vec3();
        explicit Vec3(float x, float y, float z);

        float operator[] (int index) const;
        float& operator[] (int index);
        Vec3 operator-() const;
        Vec3 & operator= (const Vec3 &a);
        Vec3 operator-(const Vec3 &a) const;
        Vec3 operator* (float a) const;

        Vec3 Cross(const Vec3 &a) const;
        float Dot(const Vec3 &a) const;

        float Normalize();
        Vec3 Normalized() const;
    };

    inline Vec3::Vec3() : x(.0f), y(.0f), z(.0f){
    }

    inline Vec3::Vec3(float x, float y, float z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    inline float Vec3::operator[] (int index) const {
        assert(index >= 0 && index < 3);
        return (&x)[index];
    }

    inline float& Vec3::operator[] (int index) {
        assert(index >= 0 && index < 3);
        return (&x)[index];
    }

    inline Vec3 Vec3::operator-() const {
        return Vec3(-x, -y, -z);
    }

    inline Vec3 &Vec3::operator= (const Vec3 &a) {
        x = a.x;
        y = a.y;
        z = a.z;

        return *this;
    }

    inline Vec3 Vec3::operator-(const Vec3 &a) const {
        return Vec3(x - a.x, y - a.y, z - a.z);
    }

    inline Vec3 Vec3::operator*(float a) const {
        return Vec3(x * a, y * a, z * a);
    }

    inline Vec3 Vec3::Cross(const Vec3 &a) const {
        return Vec3(y * a.z - z * a.y, z * a.x - x * a.z, x * a.y - y * a.x);
    }

    inline float Vec3::Dot(const Vec3 &a) const {
        return x * a.x + y * a.y + z * a.z;
    }

    inline float Vec3::Normalize() {
        float sqrLength = x * x + y * y + z * z;
        float invLength = Math::InvSqrt(sqrLength);

        x *= invLength;
        y *= invLength;
        z *= invLength;

        return invLength * sqrLength;
    }

    inline Vec3 Vec3::Normalized() const {
        float sqrLength = x * x + y * y + z * z;
        float invLength = Math::InvSqrt(sqrLength);

        return Vec3(
            x * invLength,
            y * invLength,
            z * invLength
        );
    }

    typedef Vec3 rgb;

    class Vec4 {
    public:
        float x;
        float y;
        float z;
        float w;

        Vec4();
        explicit Vec4(float x, float y, float z, float w);

        float operator[] (int index) const;
        float & operator[] (int index);
        Vec4 &operator= (const Vec4 &a);
        Vec4 operator-(const Vec4 &a) const;

        const Vec3 &ToVec3() const;
        Vec3 &ToVec3();

        const Vec2 &ToVec2() const;
        Vec2 &ToVec2();
    };

    inline Vec4::Vec4() : x(.0f), y(.0f), z(.0f), w(.0f) {
    }

    inline Vec4::Vec4(float x, float y, float z, float w) {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }

    inline float Vec4::operator[] (int index) const {
        assert(index >= 0 && index < 4);
        return (&x)[index];
    }

    inline float & Vec4::operator[] (int index) {
        assert(index >= 0 && index < 4);
        return (&x)[index];
    }

    inline Vec4 &Vec4::operator= (const Vec4 &a) {
        x = a.x;
        y = a.y;
        z = a.z;
        w = a.w;

        return *this;
    }

    inline Vec4 Vec4::operator-(const Vec4 &a) const {
        return Vec4(x - a.x, y - a.y, z - a.z, w - a.w);
    }

    inline const Vec3 & Vec4::ToVec3() const {
        return *reinterpret_cast<const Vec3 *>(this);
    }

    inline Vec3 &Vec4::ToVec3() {
        return *reinterpret_cast<Vec3 *>(this);
    }

    inline const Vec2 &Vec4::ToVec2() const {
        return *reinterpret_cast<const Vec2 *>(this);
    }

    inline Vec2 &Vec4::ToVec2() {
        return *reinterpret_cast<Vec2 *>(this);
    }
}

#endif
