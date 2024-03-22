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

RenderWorld_Soft::RenderWorld_Soft(Renderer *renderer) {
    this->renderer = dynamic_cast<SoftRenderer*>(renderer);
}

RenderWorld_Soft::~RenderWorld_Soft() {
}

void RenderWorld_Soft::RenderScene() {
    Mat4 projMat = Math::Perspective(primaryRenderView.fovY, primaryRenderView.aspect, primaryRenderView.near, primaryRenderView.far);
    Mat4 viewMat = Math::LookAt(primaryRenderView.position, primaryRenderView.target, primaryRenderView.up);

    IProgram *program = GetProgram();

    program->uniforms->projMat = projMat;
    program->uniforms->viewMat = viewMat;
    program->uniforms->vpCameraMat = projMat * viewMat;

    for (int i = 0; i < entities.Num(); i++) {
        const RenderEntity *entity = entities[i];
        RenderModel *model = entity->RenderParams().model;

        program->uniforms->modelMat = entity->Transform();

        for (int i = 0; i < model->NumSurfaces(); i++) {
            const modelSurface_t *surface = model->Surface(i);
            renderer->DrawSurface(this, surface);
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
