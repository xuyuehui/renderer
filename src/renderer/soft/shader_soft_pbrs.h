#ifndef __SHADER_SOFT_PBRS_H__
#define __SHADER_SOFT_PBRS_H__

#include "shader_soft.h"

namespace CG {

namespace Pbrs {

typedef struct shaderUniformsPbrs_t : public ishaderUniforms_t {
    Vec4 baseColor;
    float shinness;

    Texture *diffuseMap;
    Texture *specularMap;
    Texture *glossinessMap;
    Texture *normalMap;
    Texture *occlusionMap;
    Texture *emissionMap;

    float alphaCutoff;
} shaderUniformsPbrs_t;

typedef struct shaderVaryingPbrs_s : public ishaderVarying_t {
    Vec2 texcoord;
    Vec3 worldNormal;
}shaderVaryingPbrs_t;

typedef struct shaderVertexAttribsPbrs_s : public ishaderVertexAttribs_t {
    Vec2 texcoord;
    Vec3 normal;
}shaderVertexAttribsPbrs_t;

class ShaderPbrsBase : public Shader_Soft {
public:
};

class Program : public ProgramBase {
public:
    Program();
    virtual ~Program();

    void SetupMaterial(const Material *material);
    void SetupVertex(const vertex_t &v, ishaderVertexAttribs_t *attrib, ishaderVarying_t *varying);
    void Interpolate(ishaderVarying_t *in[3], const Vec3 &weights, float recipW[3], ishaderVarying_t *out);
    void Interpolate(ishaderVarying_t *src, ishaderVarying_t *dst, float ratio, ishaderVarying_t *out);
    void CopyFrom(const ishaderVarying_t *src, ishaderVarying_t *dst);
public:
    static Material *defaultMat;
    static Shader *defaultShader;

protected:
    shaderVertexAttribsPbrs_t localAttribs[3];
    shaderVaryingPbrs_t ins[MAX_VARYINGS];
    shaderVaryingPbrs_t outs[MAX_VARYINGS];
    shaderVaryingPbrs_t shaderArgs;

    shaderUniformsPbrs_t localUniforms;
};


}

}

#endif