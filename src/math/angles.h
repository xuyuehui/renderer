#ifndef __ANGLES_H__
#define __ANGLES_H__

#include "../shared.h"
#include "vector.h"
#include "quat.h"

namespace CG {

class Angles {
public:
    float pitch;
    float yaw;
    float roll;

    Angles();
    Angles(float pitch, float yaw, float roll);
    explicit Angles(const Vec3 &v);

    void Set(float pitch, float yaw, float roll);
    Angles &Zero();

    Quat ToQuat() const;
};

inline Angles::Angles() {
}

inline Angles::Angles(float pitch, float yaw, float roll) {
    this->pitch = pitch;
    this->yaw = yaw;
    this->roll = roll;
}

inline Angles::Angles(const Vec3 &v) {
    pitch = v[0];
    yaw = v[1];
    roll = v[2];
}

inline void Angles::Set(float pitch, float yaw, float roll) {
    this->pitch = pitch;
    this->yaw = yaw;
    this->roll = roll;
}

inline Angles & Angles::Zero() {
    pitch = yaw = roll = 0.0f;
    return *this;
}

}

#endif