#include "shader_lib_blinn.h"

namespace CG {

namespace Blinn {

Vec4 CommonShader::Vertex(const ishaderVertexAttribs_t *attribs, const ishaderUniforms_t *uniforms, ishaderVarying_t *varyings) const {
    const shaderVertexAttribsBlinn_t *localAttribs = static_cast<const shaderVertexAttribsBlinn_t *>(attribs);
    shaderVaryingBlinn_t *localVaryings = static_cast<shaderVaryingBlinn_t *>(varyings);
    const shaderUniformsBlinn_t *localUniforms = static_cast<const shaderUniformsBlinn_t *>(uniforms);

    if (uniforms->shadowPass) {
        return VertexShadow(localAttribs, localUniforms, localVaryings);
    }

    return VertexCommon(localAttribs, localUniforms, localVaryings);
}

Vec4 CommonShader::VertexCommon(const shaderVertexAttribsBlinn_t *attribs, const shaderUniformsBlinn_t *uniforms, shaderVaryingBlinn_t *varyings) const {
    Vec4 worldPos = uniforms->modelMat * attribs->position;
    varyings->worldPosition = worldPos.ToVec3();
    varyings->texcoord = attribs->texcoord;

    // 渲染阴影时作为shadowMap UV的索引深度值
    varyings->depthPosition = uniforms->vpLightMat * worldPos;
    varyings->normal = (uniforms->normalMat * attribs->normal).Normalized();
    Vec4 clipPosition = uniforms->vpCameraMat * worldPos;

    return clipPosition;
}

Vec4 CommonShader::VertexShadow(const shaderVertexAttribsBlinn_t *attribs, const shaderUniformsBlinn_t *uniforms, shaderVaryingBlinn_t *varyings) const {
    Vec4 worldPos = uniforms->modelMat * attribs->position;
    Vec4 depthPosition = uniforms->vpLightMat * worldPos;

    varyings->texcoord = attribs->texcoord;

    return depthPosition;
}

Vec4 CommonShader::Fragment(const ishaderVarying_t *varyings, const ishaderUniforms_t *uniforms, bool backface, bool &discard) const {
    const shaderVaryingBlinn_t *localVaryings = static_cast<const shaderVaryingBlinn_t *>(varyings);
    const shaderUniformsBlinn_t *localUniforms = static_cast<const shaderUniformsBlinn_t *>(uniforms);

    if (uniforms->shadowPass) {
        return FragmentShadow(localVaryings, localUniforms, backface, discard);
    }

    return FragmentCommon(localVaryings, localUniforms, backface, discard);
}

Vec4 CommonShader::FragmentCommon(const shaderVaryingBlinn_t *varyings, const shaderUniformsBlinn_t *uniforms, bool backface, bool &discard) const {
    Vec3 diffuse = uniforms->baseColor.ToVec3();
    float alpha = uniforms->baseColor.w;

    if (uniforms->diffuseMap) {
        Vec4 sample = Sample2D(uniforms->diffuseMap, varyings->texcoord);
        diffuse = diffuse.Mudulate(sample.ToVec3());
        alpha *= sample.w;
    }

    Vec3 specular(0, 0, 0);
    if (uniforms->specularMap) {
        Vec4 sample = Sample2D(uniforms->specularMap, varyings->texcoord);
        specular = sample.ToVec3();
    }

    float shinness = uniforms->shinness;
    Vec3 normal = varyings->normal.Normalized();
    if (backface) {
        normal = -normal;
    }

    Vec3 emission(0, 0, 0);
    if (uniforms->emissionMap) {
        Vec4 sample = Sample2D(uniforms->emissionMap, varyings->texcoord);
        emission = sample.ToVec3();
    }

    if (uniforms->alphaCutoff > 0 && alpha < uniforms->alphaCutoff) {
        discard = true;
        return Vec4(0, 0, 0, 0);
    }

    Vec3 color = emission;
    if (uniforms->ambientIntensity > 0) {
        color += (diffuse * uniforms->ambientIntensity);
    }

    if (uniforms->punctualIntensity > 0) {
        Vec3 lightDir = -uniforms->lightDir;
        float dotNL = normal.Dot(lightDir);

        if (dotNL > 0 && !IsInShadow(varyings, uniforms, dotNL)) {
            Vec3 viewDir = (uniforms->cameraPos - varyings->worldPosition).Normalized();
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

            color += (punctual * uniforms->punctualIntensity);
        }
    }

    return Vec4(color, alpha);
}

Vec4 CommonShader::FragmentShadow(const shaderVaryingBlinn_t *varyings, const shaderUniformsBlinn_t *uniforms, bool backface, bool &discard) const {
    if (uniforms->alphaCutoff > 0) {
        float alpha = uniforms->baseColor.w;
        if (uniforms->diffuseMap) {
            Vec4 sample = Sample2D(uniforms->diffuseMap, varyings->texcoord);
            alpha *= sample.w;
        }

        if (alpha < uniforms->alphaCutoff) {
            discard = true;
        }
    }

    return Vec4(0, 0, 0, 0);
}

bool CommonShader::IsInShadow(const shaderVaryingBlinn_t *varyings, const shaderUniformsBlinn_t *uniforms, float dotNL) const {
    if (!uniforms->shadowMap) {
        return false;
    }

    float u = (varyings->depthPosition.x + 1) * 0.5f;
    float v = (varyings->depthPosition.y + 1) * 0.5f;
    float d = (varyings->depthPosition.z + 1) * 0.5f;
    
    /*
    * 顶点法线和光源的夹角越大，即面相对于光源的坡度越大，更容易因为精度问题存在多个像素映射到同一个shadowmap UV，即同一个深度值，当正常渲染时采样这些点时，因为深度存在部分比shadowmap小，一部分大，
    * 在这种情况下会出现阴影瑕疵，也就是shadow acne
    * https://zhuanlan.zhihu.com/p/370951892
    * https://zhuanlan.zhihu.com/p/384446688
    */ 
    float depthBias = max(0.05f * (1 - dotNL), 0.005f);
    float currentDepth = d - depthBias;

    float closestDepth = Sample2D(uniforms->shadowMap, Vec2(u, v)).x;

    return currentDepth > closestDepth;
}

}
     
}
