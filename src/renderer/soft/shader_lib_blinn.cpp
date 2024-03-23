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

    Vec3 diffuse = localUniforms->baseColor.ToVec3();
    float alpha = localUniforms->baseColor.w;

    if (localUniforms->diffuseMap) {
        Vec4 sample = Sample2D(localUniforms->diffuseMap, localVarying->texcoord);
        diffuse = diffuse.Mudulate(sample.ToVec3());
        alpha *= sample.w;
    }

    Vec3 specular(0, 0, 0);
    if (localUniforms->specularMap) {
        Vec4 sample = Sample2D(localUniforms->specularMap, localVarying->texcoord);
        specular = sample.ToVec3();
    }

    float shinness = localUniforms->shinness;
    Vec3 normal = localVarying->normal.Normalized();

    Vec3 emission(0, 0, 0);
    if (localUniforms->emissionMap) {
        Vec4 sample = Sample2D(localUniforms->emissionMap, localVarying->texcoord);
        emission = sample.ToVec3();
    }

    if (localUniforms->alphaCutoff > 0 && alpha > localUniforms->alphaCutoff) {
        return Vec4(0, 0, 0, 0);
    }

    Vec3 color = emission;
    if (localUniforms->ambientIntensity > 0) {
        color += (diffuse * localUniforms->ambientIntensity);
    }

    return Vec4(color, alpha);
}

}
     
}
