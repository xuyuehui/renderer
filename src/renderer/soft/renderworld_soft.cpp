#include "renderworld_soft.h"
#include "../../math/maths.h"
#include "../../utility/utility.h"
#include "../render_entity.h"
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

RenderWorld_Soft::RenderWorld_Soft(Renderer *renderer) {
    this->renderer = dynamic_cast<SoftRenderer*>(renderer);
}

RenderWorld_Soft::~RenderWorld_Soft() {
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

    SortDrawSurfaces(viewMat);

    IProgram *program = GetProgram();

    program->uniforms->projMat = projMat;
    program->uniforms->viewMat = viewMat;
    program->uniforms->vpCameraMat = projMat * viewMat;

    for (int i = 0; i < drawSurfaces.Num(); i++) {
        drawSurface_t *drawSrf = &drawSurfaces[i];
        program->uniforms->modelMat = drawSrf->entity->Transform();
        renderer->DrawSurface(this, drawSrf->surface);
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
