#ifndef __QUAT_H__
#define __QUAT_H__

#include "../shared.h"

namespace CG {
    class Quat {
    public:
        float x;
        float y;
        float z;
        float w;

        Quat();
        Quat(float x, float y, float z, float w);

        float operator[] (int index) const;
        float &operator[] (int index);

        Quat &operator= (const Quat &a);

        static Quat Indentity();
    };

    inline Quat::Quat() : x(.0f), y(.0f), z(.0f), w(.0f) {
    }

    inline Quat::Quat(float x, float y, float z, float w) {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }

    inline float Quat::operator[](int index) const {
        assert(index >= 0 && index < 4);
        return (&x)[index];
    }

    inline float & Quat::operator[](int index) {
        assert(index >= 0 && index < 4);
        return (&x)[index];
    }

    inline Quat &Quat::operator=(const Quat &a){
        x = a.x;
        y = a.y;
        z = a.z;
        w = a.w;

        return *this;
    }
}

#endif
