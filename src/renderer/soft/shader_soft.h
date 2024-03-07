#ifndef __SHADER_SOFT_H__
#define __SHADER_SOFT_H__

#include "../shader.h"
#include "../../math/vector.h"
#include "../../math/matrix.h"
#include <string>

using namespace std;

namespace CG {
    class Texture;
    typedef struct vertex_s vertex_t;
    typedef struct drawSurfaceContext_s drawSurfaceContext_t;

    static const int MAX_VARYINGS = 10;
    static const int MAX_TEXTURES = 5;

    typedef struct ishaderVarying_s {
        Vec4 position;
    }ishaderVarying_t;

    typedef struct fragmentArgs_s {
        ishaderVarying_t *varying;
        Texture *textures[MAX_TEXTURES];
    }fragmentArgs_t;

    Vec4 Sample2D(const Texture *texture, const Vec2 &texcoord);

    typedef struct shaderVaryingBase_s : public ishaderVarying_t {
        Mat4 model;
        Mat4 view;
        Mat4 proj;
    }shaderVaryingBase_t;

    typedef struct shaderVaryingLocal_s : public shaderVaryingBase_t {
        Mat4 mvp;

        Vec2 texcoord;
        Vec4 color;
        Vec3 normal;
    }shaderVaryingLocal_t;

    class IProgram {
    public:
        Shader *shader;
        ishaderVarying_t *shaderVarying;

        ishaderVarying_t *inVaryings[MAX_VARYINGS];
        ishaderVarying_t *outVaryings[MAX_VARYINGS];

        bool doubleSided;

        Texture *textures[MAX_TEXTURES];
    public:
        virtual ~IProgram() {}

        virtual void Setup(ishaderVarying_t *in, const vertex_t &v, const drawSurfaceContext_t &drawContext) = 0;
        virtual void Interpolate(ishaderVarying_t *in[3], const Vec3 &weights, ishaderVarying_t *out) = 0;
        virtual void Interpolate(ishaderVarying_t *src, ishaderVarying_t *dst, float ratio, ishaderVarying_t *out) = 0;
        virtual void CopyFrom(const ishaderVarying_t *src, ishaderVarying_t *dst) = 0;
    };

    class ProgramLocal : public IProgram {
    public:
        ProgramLocal();
        virtual ~ProgramLocal();
        
        void Setup(ishaderVarying_t *in, const vertex_t &v, const drawSurfaceContext_t &drawContext);
        void Interpolate(ishaderVarying_t *in[3], const Vec3 &weights, ishaderVarying_t *out);
        void Interpolate(ishaderVarying_t *src, ishaderVarying_t *dst, float ratio, ishaderVarying_t *out);
        void CopyFrom(const ishaderVarying_t *src, ishaderVarying_t *dst);
    protected:
        shaderVaryingLocal_t ins[MAX_VARYINGS];
        shaderVaryingLocal_t outs[MAX_VARYINGS];
        shaderVaryingLocal_t shaderArgs;
    };

    class Shader_Soft : public Shader {
    public:
        virtual ~Shader_Soft() {}

        virtual void Vertex(const ishaderVarying_t *in, ishaderVarying_t *out) const = 0;
        virtual Vec4 Fragment(const fragmentArgs_t *in) const = 0;

        const char *ID() const;
    protected:
        string id;
    };

    inline const char *Shader_Soft::ID() const {
        return id.c_str();
    }
}

#endif
