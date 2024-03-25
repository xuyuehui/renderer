#include "shader_lib_pbrs.h"

namespace CG {

namespace Pbrs {

Vec4 CommonShader::Vertex(const ishaderVertexAttribs_t *attribs, const ishaderUniforms_t *uniforms, ishaderVarying_t *varyings) const {
    const shaderVertexAttribsPbrs_t *localAttribs = static_cast<const shaderVertexAttribsPbrs_t *>(attribs);
    shaderVaryingPbrs_t *localVarying = static_cast<shaderVaryingPbrs_t *>(varyings);

    localVarying->texcoord = localAttribs->texcoord;
    Vec4 clipPos = uniforms->vpCameraMat * uniforms->modelMat * localAttribs->position;

    return clipPos;
}

Vec4 CommonShader::Fragment(const ishaderVarying_t *varyings, const ishaderUniforms_t *uniforms, bool backface, bool &discard) const {
    const shaderVaryingPbrs_t *localVarying = static_cast<const shaderVaryingPbrs_t *>(varyings);
    const shaderUniformsPbrs_t *localUniforms = static_cast<const shaderUniformsPbrs_t *>(uniforms);

    return Sample2D(localUniforms->diffuseMap, localVarying->texcoord);
}

}

}