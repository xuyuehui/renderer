#include "shader_soft_pbrs.h"
#include "../material.h"
#include "../renderer.h"
#include "../model.h"

namespace CG {

namespace Pbrs {

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
    const PbrsMaterial *pbrsMat = dynamic_cast<const PbrsMaterial *>(material != NULL ? material : defaultMat);
    shader = pbrsMat->shader != NULL ? pbrsMat->shader : defaultShader;

    localUniforms.diffuseMap = pbrsMat->diffuseMap;
    localUniforms.specularMap = pbrsMat->specularMap;
    localUniforms.glossinessMap = pbrsMat->glossinessMap;
    localUniforms.normalMap = pbrsMat->normalMap;
    localUniforms.occlusionMap = pbrsMat->occlusionMap;
    localUniforms.emissionMap = pbrsMat->emissionMap;
}

void Program::SetupVertex(const vertex_t &v, ishaderVertexAttribs_t *attrib, ishaderVarying_t *varying) {
    shaderVertexAttribsPbrs_t *localAttrib = static_cast<shaderVertexAttribsPbrs_t *>(attrib);
    shaderVaryingPbrs_t *localVarying = static_cast<shaderVaryingPbrs_t *>(varying);

    localAttrib->position = Vec4(v.xyz, 1.0f);
    localAttrib->texcoord = v.st;
    localAttrib->normal = v.normal;
}

void Program::Interpolate(ishaderVarying_t *in[3], const Vec3 &weights, float recipW[3], ishaderVarying_t *out) {
    ProgramBase::InterpolateT<shaderVaryingPbrs_t>(in, weights, recipW, out);
}

void Program::Interpolate(ishaderVarying_t *src, ishaderVarying_t *dst, float ratio, ishaderVarying_t *out) {
    ProgramBase::InterpolateT<shaderVaryingPbrs_t>(src, dst, ratio, out);
}

void Program::CopyFrom(const ishaderVarying_t *src, ishaderVarying_t *dst) {
    ProgramBase::CopyFromT<shaderVaryingPbrs_t>(src, dst);
}

}

}