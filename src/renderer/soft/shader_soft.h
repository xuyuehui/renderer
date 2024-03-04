#ifndef __SHADER_SOFT_H__
#define __SHADER_SOFT_H__

#include "../shader.h"
#include "../../math/vector.h"
#include "../../math/matrix.h"
#include <string>

using namespace std;

namespace CG {
    class Texture;

    typedef struct vdata_s {
        Mat4 modelMat;
        Mat4 modelInvTransposeMat;
        Mat4 viewMat;
        Mat4 projMat;
        Mat4 vpMat;
        Mat4 mvpMat;

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
        Vec4 color;
        
        Texture *albedoTex;
        Texture *diffuseTex;
        Texture *specularTex;
        Texture *normalTex;
    }v2f_t;

    Vec4 Sample2D(const Texture *texture, const Vec2 &texcoord);

    class Shader_Soft : public Shader {
    public:
        virtual ~Shader_Soft() {}

        virtual v2f_t Vertex(const vdata_t &in) const = 0;
        virtual Vec4 Fragment(const v2f_t &in) const = 0;

        const char *ID() const;
    protected:
        string id;
    };

    inline const char *Shader_Soft::ID() const {
        return id.c_str();
    }
}

#endif
