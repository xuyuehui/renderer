#ifndef __SHADER_SOFT_PBRM_H__
#define __SHADER_SOFT_PBRM_H__

#include "shader_soft.h"

namespace CG {

namespace Pbrm {

class ShaderPbrmBase : public Shader_Soft {
public:
};

typedef struct shaderUniformsPbrm_s : public ishaderUniforms_t {
    Vec4 baseColor;
    float shinness;

    Texture *baseColorMap;
    Texture *metalnessMap;
    Texture *roughnessMap;
    Texture *normalMap;
    Texture *occlusionMap;
    Texture *emissionMap;

    float alphaCutoff;
} shaderUniformsPbrm_t;

typedef struct shaderVaryingPbrm_s : public ishaderVarying_t {
    Vec2 texcoord;
    Vec3 worldNormal;
}shaderVaryingPbrm_t;

typedef struct shaderVertexAttribsPbrm_s : public ishaderVertexAttribs_t {
    Vec2 texcoord;
    Vec3 normal;
}shaderVertexAttribsPbrm_t;

class Program : public IProgram {
public:
    Program();
    virtual ~Program();

    void SetupMaterial(const Material *material);
    void SetupVertex(const vertex_t &v, ishaderVertexAttribs_t *attrib, ishaderVarying_t *varying);
    void Interpolate(ishaderVarying_t *in[3], const Vec3 &weights, ishaderVarying_t *out);
    void Interpolate(ishaderVarying_t *src, ishaderVarying_t *dst, float ratio, ishaderVarying_t *out);
    void CopyFrom(const ishaderVarying_t *src, ishaderVarying_t *dst);
public:
    static Material *defaultMat;
    static Shader *defaultShader;

protected:
    shaderVertexAttribsPbrm_t localAttribs[3];
    shaderVaryingPbrm_t ins[MAX_VARYINGS];
    shaderVaryingPbrm_t outs[MAX_VARYINGS];
    shaderVaryingPbrm_t shaderArgs;

    shaderUniformsPbrm_t localUniforms;
};

}

}

#endif