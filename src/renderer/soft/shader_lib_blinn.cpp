#include "shader_lib_blinn.h"

namespace CG {

namespace Blinn {

Vec4 CommonShader::Vertex(const ishaderVertexAttribs_t *attribs, const ishaderUniforms_t *uniforms, ishaderVarying_t *varyings) const {
    const shaderVertexAttribsBlinn_t *localAttribs = static_cast<const shaderVertexAttribsBlinn_t *>(attribs);
    shaderVaryingBlinn_t *localVarying = static_cast<shaderVaryingBlinn_t *>(varyings);

    localVarying->worldPosition = uniforms->modelMat * attribs->position;
    localVarying->texcoord = localAttribs->texcoord;
    localVarying->depthPosition = uniforms->vpLightMat * localVarying->worldPosition;
    Vec4 clipPosition = uniforms->vpCameraMat * localVarying->worldPosition;

    return clipPosition;
}

Vec4 CommonShader::Fragment(const ishaderVarying_t *varyings, const ishaderUniforms_t *uniforms) const {
    const shaderVaryingBlinn_t *localVarying = static_cast<const shaderVaryingBlinn_t *>(varyings);
    const shaderUniformsBlinn_t *localUniforms = static_cast<const shaderUniformsBlinn_t *>(uniforms);
    return Sample2D(localUniforms->diffuseMap, localVarying->texcoord);
}

}
     
}
