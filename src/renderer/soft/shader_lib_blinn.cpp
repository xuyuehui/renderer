#include "shader_lib_blinn.h"

namespace CG {

namespace Blinn {

Vec4 CommonShader::Vertex(const ishaderVertexAttribs_t *attribs, const ishaderUniforms_t *uniforms, ishaderVarying_t *varyings) const {
    const shaderVertexAttribsBlinn_t *localAttribs = static_cast<const shaderVertexAttribsBlinn_t *>(attribs);
    shaderVaryingBlinn_t *localVarying = static_cast<shaderVaryingBlinn_t *>(varyings);

    Vec4 worldPos = uniforms->modelMat * attribs->position;
    localVarying->worldPosition = worldPos.ToVec3();
    localVarying->texcoord = localAttribs->texcoord;
    localVarying->depthPosition = uniforms->vpLightMat * worldPos;
    localVarying->normal = uniforms->normalMat * localAttribs->normal;
    Vec4 clipPosition = uniforms->vpCameraMat * worldPos;

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

    if (localUniforms->punctualIntensity > 0) {
        Vec3 lightDir = -localUniforms->lightDir;
        float dotNL = normal.Dot(lightDir);

        if (dotNL > 0) {
            Vec3 viewDir = (localUniforms->cameraPos - localVarying->worldPosition).Normalized();
            if (!specular.IsZero()) {
                Vec3 halfDir = (lightDir + viewDir).Normalized();
                float dotNH = normal.Dot(halfDir);
                if (dotNH > 0) {
                    float strength = pow(dotNH, shinness);
                    specular *= strength;
                }
            }

            diffuse *= dotNL;
            Vec3 punctual = diffuse + specular;

            color += (punctual * localUniforms->punctualIntensity);
        }
    }

    return Vec4(color, alpha);
}

}
     
}
