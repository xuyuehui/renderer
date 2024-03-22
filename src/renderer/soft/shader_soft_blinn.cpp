#include "shader_soft_blinn.h"
#include "../material.h"
#include "../renderer.h"
#include "../model.h"

namespace CG {

namespace Blinn {

Material *Program::defaultMat = NULL;
Shader *Program::defaultShader = NULL;

Program::Program() {
    for (int i = 0; i < 3; i++) {
        attribs[i] = &localAttribs[i];
    }

    for (int i = 0; i < MAX_VARYINGS; i++) {
        inVaryings[i] = &ins[i];
        outVaryings[i] = &outs[i];
    }

    shaderVarying = &shaderArgs;
    uniforms = &localUniforms;
}

Program::~Program() {
}

void Program::SetupMaterial(const Material *material) {
    const BlinnMaterial *blinnMat = dynamic_cast<const BlinnMaterial *>(material != NULL ? material : defaultMat);
    shader = blinnMat->shader != NULL ? blinnMat->shader : defaultShader;

    localUniforms.baseColor = blinnMat->baseColor;
    localUniforms.shinness = blinnMat->shininess;
    localUniforms.alphaCutoff = blinnMat->alphaCutoff;

    localUniforms.diffuseMap = blinnMat->diffuseMap;
    localUniforms.specularMap = blinnMat->specularMap;
    localUniforms.normalMap = blinnMat->normalMap;
    localUniforms.emissionMap = blinnMat->emissionMap;
}

void Program::SetupVertex(const vertex_t &v, ishaderVertexAttribs_t *attrib, ishaderVarying_t *varying) {
    shaderVertexAttribsBlinn_t *localAttrib = static_cast<shaderVertexAttribsBlinn_t *>(attrib);
    shaderVaryingBlinn_t *localVarying = static_cast<shaderVaryingBlinn_t *>(varying);

    localAttrib->position = Vec4(v.xyz, 1.0f);
    localAttrib->texcoord = v.st;
    localAttrib->normal = v.normal;
}

void Program::Interpolate(ishaderVarying_t *in[3], const Vec3 &weights, ishaderVarying_t *out) {
    int numOfFloats = sizeof(shaderVaryingBlinn_t) / sizeof(float);
    float *dst = reinterpret_cast<float *>(out);
    float *src0 = reinterpret_cast<float *>(in[0]);
    float *src1 = reinterpret_cast<float *>(in[1]);
    float *src2 = reinterpret_cast<float *>(in[2]);

    for (int i = 0; i < numOfFloats; i++) {
        dst[i] = src0[i] * weights.x + src1[i] * weights.y + src2[i] * weights.z;
    }
}

void Program::Interpolate(ishaderVarying_t *src, ishaderVarying_t *dst, float ratio, ishaderVarying_t *out) {
    int numOfFloats = sizeof(shaderVaryingBlinn_t) / sizeof(float);
    float *outf = reinterpret_cast<float *>(out);
    float *srcf = reinterpret_cast<float *>(src);
    float *dstf = reinterpret_cast<float *>(dst);

    for (int i = 0; i < numOfFloats; i++) {
        outf[i] = srcf[i] * ratio + dstf[i] * (1.0f - ratio);
    }
}

void Program::CopyFrom(const ishaderVarying_t *src, ishaderVarying_t *dst) {
    memcpy((void *)dst, src, sizeof(shaderVaryingBlinn_t));
}

}

}