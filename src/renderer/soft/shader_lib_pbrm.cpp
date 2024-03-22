#include "shader_lib_pbrm.h"

namespace CG {

namespace Pbrm {

Vec4 CommonShader::Vertex(const ishaderVertexAttribs_t *attribs, const ishaderUniforms_t *uniforms, ishaderVarying_t *varyings) const {
    const shaderVertexAttribsPbrm_t *localAttribs = static_cast<const shaderVertexAttribsPbrm_t *>(attribs);
    shaderVaryingPbrm_t *localVarying = static_cast<shaderVaryingPbrm_t *>(varyings);

    localVarying->texcoord = localAttribs->texcoord;
    Vec4 clipPos = uniforms->vpCameraMat * uniforms->modelMat * localAttribs->position;

    return clipPos;
}

Vec4 CommonShader::Fragment(const ishaderVarying_t *varyings, const ishaderUniforms_t *uniforms) const {
    const shaderVaryingPbrm_t *localVarying = static_cast<const shaderVaryingPbrm_t *>(varyings);
    const shaderUniformsPbrm_t *localUniforms = static_cast<const shaderUniformsPbrm_t *>(uniforms);

    return Sample2D(localUniforms->baseColorMap, localVarying->texcoord);
}

}

}