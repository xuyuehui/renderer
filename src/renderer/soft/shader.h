#ifndef __SHADER_H__
#define __SHADER_H__

#include "../../shared.h"
#include "../../math/vector.h"
#include "../../math/matrix.h"

namespace CG {

    typedef struct vdata_s {
        Mat4 localMat;
        Mat4 vpMat;
        Vec3 position;
        Vec3 normal;
        Vec2 texcoord;
        Vec4 color;
        Vec3 tangent;
        Vec3 bitangent;
    }vdata_t;

    typedef struct v2f_s {
        Vec4 position;
        Vec3 fragPos;
        Vec3 normal;
        Vec3 t_normal;
        Vec2 texcoord;
        Vec3 tangent;
        Vec3 bitangent;
    }v2f_t;

    class Shader {
    public:
        virtual ~Shader() {}

        virtual const char *Name() const = 0;

        virtual v2f_t Vertex(const vdata_t &in) const = 0;
        virtual Vec4 Fragment(const v2f_t &in) const = 0;
    };
}

#endif