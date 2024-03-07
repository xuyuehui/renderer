//
//  shader_soft.cpp
//  renderer
//
//  Created by xuyuehui on 2024/3/4.
//

#include "shader_soft.h"
#include "../texture.h"
#include "../model.h"
#include "../renderer.h"

namespace CG {
    Vec4 Sample2D(const Texture *texture, const Vec2 &texcoord) {
        if (texture == NULL) {
            return Vec4(1.0f, .0f, 1.0f, 1);
        }
        
        if (texture->GetFilterType() == TEXFT_LINEAR) {
            int w = 0, h = 0;
            texture->GetSize(w, h);
            
            float x = clamp(texcoord.x, .0f, 1.0f) * w;
            float y = clamp(texcoord.y, .0f, 1.0f) * h;
            
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
            
            int x = floor(clamp(texcoord.x, .0f, 1.0f) * w);
            int y = floor(clamp(texcoord.y, .0f, 1.0f) * h);
            
            return texture->ColorAt(x, y);
        }
    }

    ProgramLocal::ProgramLocal() {
        for (int i = 0; i < MAX_VARYINGS; i++) {
            inVaryings[i] = &ins[i];
            outVaryings[i] = &outs[i];
        }

        shaderVarying = &shaderArgs;

        memset(textures, 0, sizeof(textures));
    }

    ProgramLocal::~ProgramLocal() {
    }

    void ProgramLocal::Setup(ishaderVarying_t *in, const vertex_t &v, const drawSurfaceContext_t &drawContext) {
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

    void ProgramLocal::Interpolate(ishaderVarying_t *in[3], const Vec3 &weights, ishaderVarying_t *out) {
        int numOfFloats = sizeof(shaderVaryingLocal_t) / sizeof(float);
        float *dst = reinterpret_cast<float *>(out);
        float *src0 = reinterpret_cast<float *>(in[0]);
        float *src1 = reinterpret_cast<float *>(in[1]);
        float *src2 = reinterpret_cast<float *>(in[2]);

        for (int i = 0; i < numOfFloats; i++) {
            dst[i] = src0[i] * weights.x + src1[i] * weights.y + src2[i] * weights.z;
        }
    }

    void ProgramLocal::Interpolate(ishaderVarying_t *src, ishaderVarying_t *dst, float ratio, ishaderVarying_t *out) {
        int numOfFloats = sizeof(shaderVaryingLocal_t) / sizeof(float);
        float *outf = reinterpret_cast<float *>(out);
        float *srcf = reinterpret_cast<float *>(src);
        float *dstf = reinterpret_cast<float *>(dst);

        for (int i = 0; i < numOfFloats; i++) {
            outf[i] = srcf[i] * ratio + dstf[i] * (1.0f - ratio);
        }
    }

    void ProgramLocal::CopyFrom(const ishaderVarying_t *src, ishaderVarying_t *dst) {
        memcpy((void*)dst, src, sizeof(shaderVaryingLocal_t));
    }

}
