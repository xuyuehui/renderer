#include "shader_soft_pbrm.h"
#include "../material.h"
#include "../renderer.h"
#include "../model.h"

namespace CG {

namespace Pbrm {

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
    const PbrmMaterial *pbrmMat = dynamic_cast<const PbrmMaterial *>(material != NULL ? material : defaultMat);
    shader = pbrmMat->shader != NULL ? pbrmMat->shader : defaultShader;

    localUniforms.baseColorMap = pbrmMat->baseColorMap;
    localUniforms.metalnessMap = pbrmMat->metalnessMap;
    localUniforms.roughnessMap = pbrmMat->roughnessMap;
    localUniforms.normalMap = pbrmMat->normalMap;
    localUniforms.occlusionMap = pbrmMat->occlusionMap;
    localUniforms.emissionMap = pbrmMat->emissionMap;
}

void Program::SetupVertex(const vertex_t &v, ishaderVertexAttribs_t *attrib, ishaderVarying_t *varying) {
    shaderVertexAttribsPbrm_t *localAttrib = static_cast<shaderVertexAttribsPbrm_t *>(attrib);
    shaderVaryingPbrm_t *localVarying = static_cast<shaderVaryingPbrm_t *>(varying);

    localAttrib->position = Vec4(v.xyz, 1.0f);
    localAttrib->texcoord = v.st;
    localAttrib->normal = v.normal;
}

void Program::Interpolate(ishaderVarying_t *in[3], const Vec3 &weights, ishaderVarying_t *out) {
    int numOfFloats = sizeof(shaderVaryingPbrm_t) / sizeof(float);
    float *dst = reinterpret_cast<float *>(out);
    float *src0 = reinterpret_cast<float *>(in[0]);
    float *src1 = reinterpret_cast<float *>(in[1]);
    float *src2 = reinterpret_cast<float *>(in[2]);

    for (int i = 0; i < numOfFloats; i++) {
        dst[i] = src0[i] * weights.x + src1[i] * weights.y + src2[i] * weights.z;
    }
}

void Program::Interpolate(ishaderVarying_t *src, ishaderVarying_t *dst, float ratio, ishaderVarying_t *out) {
    int numOfFloats = sizeof(shaderVaryingPbrm_t) / sizeof(float);
    float *outf = reinterpret_cast<float *>(out);
    float *srcf = reinterpret_cast<float *>(src);
    float *dstf = reinterpret_cast<float *>(dst);

    for (int i = 0; i < numOfFloats; i++) {
        outf[i] = srcf[i] * ratio + dstf[i] * (1.0f - ratio);
    }
}

void Program::CopyFrom(const ishaderVarying_t *src, ishaderVarying_t *dst) {
    memcpy((void *)dst, src, sizeof(shaderVaryingPbrm_t));
}


}

}