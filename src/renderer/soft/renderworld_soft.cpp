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

namespace CG {
    RenderWorld_Soft::RenderWorld_Soft(Renderer *renderer) {
        this->renderer = dynamic_cast<SoftRenderer*>(renderer);
    }

    RenderWorld_Soft::~RenderWorld_Soft() {
    }

    void RenderWorld_Soft::RenderScene() {
        Mat4 projMat = Math::ProjectMatrix(primaryRenderView.fovY, primaryRenderView.aspect, primaryRenderView.near, primaryRenderView.far);
        Mat4 viewMat = Math::ViewMatrix(primaryRenderView.position, primaryRenderView.target, primaryRenderView.up);

        SoftRenderer::drawSurfaceContext_t drawSrfContext;

        drawSrfContext.proj = projMat;
        drawSrfContext.view = viewMat;

        for (int i = 0; i < entities.Num(); i++) {
            const RenderEntity *entity = entities[i];
            RenderModel *model = entity->RenderParams().model;

            drawSrfContext.model = entity->Transform();

            for (int i = 0; i < model->NumSurfaces(); i++) {
                const modelSurface_t *surface = model->Surface(i);
                renderer->DrawSurface(surface, drawSrfContext);
            }
        }
    }
}
