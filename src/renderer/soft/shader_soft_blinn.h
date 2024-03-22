#ifndef __SHADER_SOFT_BLINN_H__
#define __SHADER_SOFT_BLINN_H__

#include "shader_soft.h"

namespace CG {

namespace Blinn {

typedef struct shaderVaryingBlinn_s : public ishaderVarying_t {
    Vec4 worldPosition;
    Vec4 depthPosition;
    Vec2 texcoord;
    Vec3 normal;
}shaderVaryingBlinn_t;

typedef struct shaderUniformsBlinn_s : public ishaderUniforms_t {
    Vec4 baseColor;
    float shinness;

    Texture *diffuseMap;
    Texture *specularMap;
    Texture *normalMap;
    Texture *emissionMap;
    
    float alphaCutoff;
} shaderUniformsBlinn_t;

typedef struct shaderVertexAttribsBlinn_s : public ishaderVertexAttribs_t {
    Vec2 texcoord;
    Vec3 normal;
} shaderVertexAttribsBlinn_t;

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
    shaderVertexAttribsBlinn_t localAttribs[3];
    shaderVaryingBlinn_t ins[MAX_VARYINGS];
    shaderVaryingBlinn_t outs[MAX_VARYINGS];
    shaderVaryingBlinn_t shaderArgs;

    shaderUniformsBlinn_t localUniforms;
};

class ShaderBlinnBase : public Shader_Soft {
protected:
};

}

}

#endif