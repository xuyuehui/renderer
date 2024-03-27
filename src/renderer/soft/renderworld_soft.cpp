#include "renderworld_soft.h"
#include "../../math/maths.h"
#include "../../utility/utility.h"
#include "../render_entity.h"
#include "../render_light.h"
#include "../model.h"
#include "../material.h"
#include "../../shared.h"

#include "soft_renderer.h"
#include "shadermanager_soft.h"
#include "shader_soft.h"
#include "msaa.h"
#include "frame_buffer.h"

#include "shader_soft_blinn.h"
#include "shader_soft_pbrm.h"
#include "shader_soft_pbrs.h"

#include "../texture.h"
#include "frame_buffer.h"

namespace CG {

int DrawSurfaceSortCompare(const RenderWorld_Base::drawSurface_t *a, const RenderWorld_Base::drawSurface_t *b) {
    if (a->surface->material->opaque && b->surface->material->opaque) {
        return a->distance < b->distance ? -1 : 1;
    }
    else if (a->surface->material->opaque && !b->surface->material->opaque) {
        return -1;
    }
    else if (!a->surface->material->opaque && b->surface->material->opaque) {
        return 1;
    }

    return a->distance < b->distance ? 1 : -1;
}

inline void CopyDepthBuffer2Texture(const FrameBuffer *frameBuffer, Texture *texture) {
    int numPixels = frameBuffer->GetWidth() * frameBuffer->GetHeight();

    assert(frameBuffer->GetWidth() == texture->GetWidth());
    assert(frameBuffer->GetHeight() == texture->GetHeight());

    for (int i = 0; i < numPixels; i++) {
        float depth = frameBuffer->GetDepthBuffer()[i];
        texture->SetColor(i, Vec4(depth, depth, depth, 1));
    }
}

RenderWorld_Soft::RenderWorld_Soft(Renderer *renderer) : shadowBuffer(NULL), shadowMap(NULL){
    this->renderer = dynamic_cast<SoftRenderer*>(renderer);
}

RenderWorld_Soft::~RenderWorld_Soft() {
    delete shadowBuffer;
    delete shadowMap;
}

// 在摄像机空间下，进行Z值排序
void RenderWorld_Soft::SortDrawSurfaces(const Mat4 &viewMat) {
    drawSurfaces.Clear();
    for (int i = 0; i < entities.Num(); i++) {
        RenderModel *model = entities[i]->RenderParams().model;
        const Mat4 &modelMat = entities[i]->Transform();

        for (int j = 0; j < model->NumSurfaces(); j++) {
            const modelSurface_t *surface = model->Surface(j);

            if (surface && surface->geometry) {
                Vec4 localPos(surface->center, 1);
                Vec4 viewPos = viewMat * modelMat * localPos;

                drawSurfaces.Append({ entities[i], surface, -viewPos.z });
            }
        }
    }

    drawSurfaces.Sort((List<drawSurface_t>::cmp_t *) & DrawSurfaceSortCompare);
}

void RenderWorld_Soft::RenderScene() {
    Mat4 projMat = Math::Perspective(primaryRenderView.fovY, primaryRenderView.aspect, primaryRenderView.near, primaryRenderView.far);
    Mat4 viewMat = Math::LookAt(primaryRenderView.position, primaryRenderView.target, primaryRenderView.up);

    IProgram *program = GetProgram();

    program->uniforms->projMat = projMat;
    program->uniforms->viewMat = viewMat;
    program->uniforms->vpCameraMat = projMat * viewMat;
    program->uniforms->cameraPos = primaryRenderView.position;

    if (lights.Num() > 0) {
        program->uniforms->lightDir = lights[0]->RenderParams().dir;
        program->uniforms->punctualIntensity = lights[0]->RenderParams().punctual;
    }
    else {
        program->uniforms->lightDir.Zero();
        program->uniforms->punctualIntensity = 0.0f;
    }

    // 渲染阴影
    if (shadowBuffer && shadowMap) {
        Vec3 lightPos = -program->uniforms->lightDir;
        Vec3 lightTarget = Vec3(0, 0, 0);
        Vec3 lightUp = Vec3(0, 1, 0);
        Mat4 lightViewMat = Math::LookAt(lightPos, lightTarget, lightUp);
        Mat4 lightProjMat = Math::Orthographic(1, 1, 0, 2);
        program->uniforms->vpLightMat = lightProjMat * lightViewMat;
        
        shadowBuffer->ClearDepthBuffer(1.0f);

        SortDrawSurfaces(lightViewMat);
        for (int i = 0; i < drawSurfaces.Num(); i++) {
            drawSurface_t *drawSrf = &drawSurfaces[i];
            if (drawSrf->surface->material && drawSrf->surface->material->opaque) {
                program->uniforms->modelMat = drawSrf->entity->Transform();
                program->uniforms->normalMat = drawSrf->entity->Transform().Inverse().Transport();
                renderer->DrawSurface(this, drawSrf->surface, true, shadowBuffer);
            }
        }

        CopyDepthBuffer2Texture(shadowBuffer, shadowMap);

        program->uniforms->shadowMap = shadowMap;
    }

    // 根据摄像机位置排序
    SortDrawSurfaces(viewMat);

    // 正常渲染
    for (int i = 0; i < drawSurfaces.Num(); i++) {
        drawSurface_t *drawSrf = &drawSurfaces[i];
        program->uniforms->modelMat = drawSrf->entity->Transform();
        program->uniforms->normalMat = drawSrf->entity->Transform().Inverse().Transport();
        renderer->DrawSurface(this, drawSrf->surface, false, NULL);
    }
}

void RenderWorld_Soft::SetShadowSize(int width, int height) {
    if (width > 0 && height > 0) {
        if (!shadowBuffer || (shadowBuffer && (shadowBuffer->GetWidth() != width || shadowBuffer->GetHeight() != height))) {
            if (shadowBuffer) {
                delete shadowBuffer;
            }

            shadowBuffer = new FrameBuffer(width, height);
        }

        if (!shadowMap || (shadowMap && (shadowMap->GetWidth() != width || shadowMap->GetHeight() != height))) {
            if (shadowMap) {
                delete shadowMap;
            }

            shadowMap = new Texture(width, height);
        }
    }
    else {
        if (shadowBuffer) {
            delete shadowBuffer;
            shadowBuffer = NULL;
        }

        if (shadowMap) {
            delete shadowMap;
            shadowMap = NULL;
        }
    }
}

RenderWorldBlinn_Soft::RenderWorldBlinn_Soft(Renderer *renderer) : RenderWorld_Soft(renderer) {
    program = new Blinn::Program();
}

RenderWorldBlinn_Soft::~RenderWorldBlinn_Soft() {
    delete program;
}

IProgram *RenderWorldBlinn_Soft::GetProgram() const {
    return program;
}

RenderWorldPbrm_Soft::RenderWorldPbrm_Soft(Renderer *renderer) : RenderWorld_Soft(renderer) {
    program = new Pbrm::Program();
}

RenderWorldPbrm_Soft::~RenderWorldPbrm_Soft() {
    delete program;
}

IProgram *RenderWorldPbrm_Soft::GetProgram() const {
    return program;
}

RenderWorldPbrs_Soft::RenderWorldPbrs_Soft(Renderer *renderer) : RenderWorld_Soft(renderer) {
    program = new Pbrs::Program();
}

RenderWorldPbrs_Soft::~RenderWorldPbrs_Soft() {
    delete program;
}

IProgram *RenderWorldPbrs_Soft::GetProgram() const {
    return program;
}

}
