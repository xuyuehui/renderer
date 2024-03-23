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

    localUniforms.ambientIntensity = 0.5f;
}

void Program::SetupVertex(const vertex_t &v, ishaderVertexAttribs_t *attrib, ishaderVarying_t *varying) {
    shaderVertexAttribsBlinn_t *localAttrib = static_cast<shaderVertexAttribsBlinn_t *>(attrib);
    shaderVaryingBlinn_t *localVarying = static_cast<shaderVaryingBlinn_t *>(varying);

    localAttrib->position = Vec4(v.xyz, 1.0f);
    localAttrib->texcoord = v.st;
    localAttrib->normal = v.normal;
}

void Program::Interpolate(ishaderVarying_t *in[3], const Vec3 &weights, float recipW[3], ishaderVarying_t *out) {
    ProgramBase::InterpolateT<shaderVaryingBlinn_t>(in, weights, recipW, out);
}

void Program::Interpolate(ishaderVarying_t *src, ishaderVarying_t *dst, float ratio, ishaderVarying_t *out) {
    ProgramBase::InterpolateT<shaderVaryingBlinn_t>(src, dst, ratio, out);
}

void Program::CopyFrom(const ishaderVarying_t *src, ishaderVarying_t *dst) {
    ProgramBase::CopyFromT<shaderVaryingBlinn_t>(src, dst);
}

}

}