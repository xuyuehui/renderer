//
//  shader_soft.cpp
//  renderer
//
//  Created by xuyuehui on 2024/3/4.
//

#include "shader_soft.h"
#include "../texture.h"
#include "../model.h"
#include "../material.h"
#include "../renderer.h"

namespace CG {

Vec4 Shader_Soft::Sample2D(const Texture *texture, const Vec2 &texcoord) {
    if (texture == NULL) {
        return Vec4(1.0f, .0f, 1.0f, 1);
    }
        
    if (texture->GetFilterType() == TEXFT_LINEAR) {
        int w = 0, h = 0;
        texture->GetSize(w, h);
            
        float x = clamp(texcoord.x, .0f, 1.0f) * (w-1);
        float y = clamp(texcoord.y, .0f, 1.0f) * (h-1);
            
        float stepX = x - floor(x);
        float stepY = y - floor(y);
            
        Vec4 color0 = texture->ColorAt(x, y);
        Vec4 color1 = texture->ColorAt(x + 1, y);
        Vec4 color2 = texture->ColorAt(x, y + 1);
        Vec4 color3 = texture->ColorAt(x + 1, y + 1);
            
        return Vec4::Lerp(Vec4::Lerp(color0, color1, stepX), 
                            Vec4::Lerp(color2, color3, stepX),
                            stepY);
    } else {
        int w = 0, h = 0;
        texture->GetSize(w, h);
            
        int x = floor(clamp(texcoord.x, .0f, 1.0f) * (w-1));
        int y = floor(clamp(texcoord.y, .0f, 1.0f) * (h-1));
            
        return texture->ColorAt(x, y);
    }
}

void ProgramBase::Interpolate(ishaderVarying_t *in[3], const Vec3 &weights, ishaderVarying_t *out) {
    int numOfFloats = sizeof(shaderVaryingLocal_t) / sizeof(float);
    float *dst = reinterpret_cast<float *>(out);
    float *src0 = reinterpret_cast<float *>(in[0]);
    float *src1 = reinterpret_cast<float *>(in[1]);
    float *src2 = reinterpret_cast<float *>(in[2]);

    for (int i = 0; i < numOfFloats; i++) {
        dst[i] = src0[i] * weights.x + src1[i] * weights.y + src2[i] * weights.z;
    }
}

void ProgramBase::Interpolate(ishaderVarying_t *src, ishaderVarying_t *dst, float ratio, ishaderVarying_t *out) {
    int numOfFloats = sizeof(shaderVaryingLocal_t) / sizeof(float);
    float *outf = reinterpret_cast<float *>(out);
    float *srcf = reinterpret_cast<float *>(src);
    float *dstf = reinterpret_cast<float *>(dst);

    for (int i = 0; i < numOfFloats; i++) {
        outf[i] = srcf[i] * ratio + dstf[i] * (1.0f - ratio);
    }
}

void ProgramBase::CopyFrom(const ishaderVarying_t *src, ishaderVarying_t *dst) {
    memcpy((void *)dst, src, sizeof(shaderVaryingLocal_t));
}

Material *ProgramBlinn::defaultMat = NULL;
Shader *ProgramBlinn::defaultShader = NULL;

ProgramBlinn::ProgramBlinn() {
    for (int i = 0; i < MAX_VARYINGS; i++) {
        inVaryings[i] = &ins[i];
        outVaryings[i] = &outs[i];
    }

    shaderVarying = &shaderArgs;

    memset(textures, 0, sizeof(textures));
}

ProgramBlinn::~ProgramBlinn() {
}

void ProgramBlinn::SetupMaterial(const Material *material) {
    const BlinnMaterial *blinnMat = dynamic_cast<const BlinnMaterial *>(material != NULL ? material : defaultMat);
    shader = blinnMat->shader != NULL ? blinnMat->shader : defaultShader;

    textures[0] = blinnMat->diffuseMap;
    textures[1] = blinnMat->specularMap;
    textures[2] = blinnMat->normalMap;
    textures[3] = blinnMat->emissionMap;
}

void ProgramBlinn::SetupVertex(ishaderVarying_t *in, const vertex_t &v, const drawSurfaceContext_t &drawContext) {
    shaderVaryingLocal_t *inVarying = static_cast<shaderVaryingLocal_t *>(in);

    inVarying->model = drawContext.model;
    inVarying->view = drawContext.view;
    inVarying->proj = drawContext.proj;

    inVarying->mvp = inVarying->proj * inVarying->view * inVarying->model;
    inVarying->position = Vec4(v.xyz, 1.0f);
    inVarying->texcoord = v.st;
    inVarying->normal = v.normal;
    inVarying->color = v.color;
}

Material *ProgramPbrm::defaultMat = NULL;
Shader *ProgramPbrm::defaultShader = NULL;

ProgramPbrm::ProgramPbrm() {
    for (int i = 0; i < MAX_VARYINGS; i++) {
        inVaryings[i] = &ins[i];
        outVaryings[i] = &outs[i];
    }

    shaderVarying = &shaderArgs;

    memset(textures, 0, sizeof(textures));
}

ProgramPbrm::~ProgramPbrm() {
}

void ProgramPbrm::SetupMaterial(const Material *material) {
    const PbrmMaterial *pbrmMat = dynamic_cast<const PbrmMaterial *>(material != NULL ? material : defaultMat);
    shader = pbrmMat->shader != NULL ? pbrmMat->shader : defaultShader;

    textures[0] = pbrmMat->baseColorMap;
    textures[1] = pbrmMat->metalnessMap;
    textures[2] = pbrmMat->roughnessMap;
    textures[3] = pbrmMat->normalMap;
    textures[4] = pbrmMat->occlusionMap;
    textures[5] = pbrmMat->emissionMap;
}

void ProgramPbrm::SetupVertex(ishaderVarying_t *in, const vertex_t &v, const drawSurfaceContext_t &drawContext) {
    shaderVaryingLocal_t *inVarying = static_cast<shaderVaryingLocal_t *>(in);

    inVarying->model = drawContext.model;
    inVarying->view = drawContext.view;
    inVarying->proj = drawContext.proj;

    inVarying->mvp = inVarying->proj * inVarying->view * inVarying->model;
    inVarying->position = Vec4(v.xyz, 1.0f);
    inVarying->texcoord = v.st;
    inVarying->normal = v.normal;
    inVarying->color = v.color;
}

Material *ProgramPbrs::defaultMat = NULL;
Shader *ProgramPbrs::defaultShader = NULL;

ProgramPbrs::ProgramPbrs() {
    for (int i = 0; i < MAX_VARYINGS; i++) {
        inVaryings[i] = &ins[i];
        outVaryings[i] = &outs[i];
    }

    shaderVarying = &shaderArgs;

    memset(textures, 0, sizeof(textures));
}

ProgramPbrs::~ProgramPbrs() {
}

void ProgramPbrs::SetupMaterial(const Material *material) {
    const PbrsMaterial *pbrsMat = dynamic_cast<const PbrsMaterial *>(material != NULL ? material : defaultMat);
    shader = pbrsMat->shader != NULL ? pbrsMat->shader : defaultShader;

    textures[0] = pbrsMat->diffuseMap;
    textures[1] = pbrsMat->specularMap;
    textures[2] = pbrsMat->glossinessMap;
    textures[3] = pbrsMat->normalMap;
    textures[4] = pbrsMat->occlusionMap;
    textures[5] = pbrsMat->emissionMap;
}

void ProgramPbrs::SetupVertex(ishaderVarying_t *in, const vertex_t &v, const drawSurfaceContext_t &drawContext) {
    shaderVaryingLocal_t *inVarying = static_cast<shaderVaryingLocal_t *>(in);

    inVarying->model = drawContext.model;
    inVarying->view = drawContext.view;
    inVarying->proj = drawContext.proj;

    inVarying->mvp = inVarying->proj * inVarying->view * inVarying->model;
    inVarying->position = Vec4(v.xyz, 1.0f);
    inVarying->texcoord = v.st;
    inVarying->normal = v.normal;
    inVarying->color = v.color;
}

}
