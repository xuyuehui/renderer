#ifndef __SHADER_SOFT_H__
#define __SHADER_SOFT_H__

#include "../shader.h"
#include "../../math/vector.h"
#include "../../math/matrix.h"
#include <string>

using namespace std;

namespace CG {

class Texture;
class Material;
typedef struct vertex_s vertex_t;
typedef struct drawSurfaceContext_s drawSurfaceContext_t;

static const int MAX_VARYINGS = 10;
static const int MAX_TEXTURES = 8;

typedef struct ishaderVarying_s {
}ishaderVarying_t;

typedef struct ishaderVertexAttribs_s {
    Vec4 position;
} ishaderVertexAttribs_t;

typedef struct ishaderUniforms_s {
    Mat4 modelMat;
    Mat4 viewMat;
    Mat4 projMat;
    Mat4 vpCameraMat;
    Mat4 vpLightMat;
}ishaderUniforms_t;

class IProgram {
public:
    Shader *shader;
    ishaderVarying_t *shaderVarying;

    ishaderVertexAttribs_t *attribs[3];

    Vec4 inCoords[MAX_VARYINGS];
    Vec4 outCoords[MAX_VARYINGS];

    ishaderVarying_t *inVaryings[MAX_VARYINGS];
    ishaderVarying_t *outVaryings[MAX_VARYINGS];

    ishaderUniforms_t *uniforms;

    bool doubleSided;
public:
    virtual ~IProgram() {}

    virtual void SetupMaterial(const Material *material) = 0;
    virtual void SetupVertex(const vertex_t &v, ishaderVertexAttribs_t *attrib, ishaderVarying_t *varying) = 0;
    virtual void Interpolate(ishaderVarying_t *in[3], const Vec3 &weights, ishaderVarying_t *out) = 0;
    virtual void Interpolate(ishaderVarying_t *src, ishaderVarying_t *dst, float ratio, ishaderVarying_t *out) = 0;
    virtual void CopyFrom(const ishaderVarying_t *src, ishaderVarying_t *dst) = 0;
};

class Shader_Soft : public Shader {
public:
    virtual ~Shader_Soft() {}

    virtual Vec4 Vertex(const ishaderVertexAttribs_t *attribs, const ishaderUniforms_t *uniforms, ishaderVarying_t *varyings) const = 0;
    virtual Vec4 Fragment(const ishaderVarying_t *in, const ishaderUniforms_t *uniforms) const = 0;

    static Vec4 Sample2D(const Texture *texture, const Vec2 &texcoord);
};

}

#endif
