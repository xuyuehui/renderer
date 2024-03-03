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

    static inline vdata_t VDATA(const srfTriangles_t *triangles, int vertIndex, const Mat4& model, const Mat4& view, const Mat4 &proj, const Mat4& vp, const Mat4 &mvp, const Mat4& modelInvTranspose) {
        vdata_t vd;

        vd.modelMat = model;
        vd.viewMat = view;
        vd.projMat = proj;
        vd.vpMat = vp;
        vd.mvpMat = mvp;
        vd.modelInvTransposeMat = modelInvTranspose;

        vd.position = triangles->verts[triangles->indexes[vertIndex]].xyz;
        vd.normal = triangles->verts[triangles->indexes[vertIndex]].normal;
        vd.texcoord = triangles->verts[triangles->indexes[vertIndex]].st;
        vd.color = triangles->verts[triangles->indexes[vertIndex]].color;
        vd.tangent = triangles->verts[triangles->indexes[vertIndex]].tangent;

        return vd;
    }

    static inline float ScreenMapping_X(float x, int width) {
        return floor((x * 0.5f + 0.5f) * width);
    }

    static inline float ScreenMapping_Y(float y, int height) {
        return floor((y * 0.5f + 0.5f) * height);
    }

    static inline byte Float2ByteColor(float v) {
        return (byte)(clamp(v, 0.0f, 1.0f) * 255);
    }

    RenderWorld_Soft::RenderWorld_Soft(Renderer *renderer) {
        this->renderer = dynamic_cast<SoftRenderer*>(renderer);

        defaultShader = this->renderer->GetShaderManager()->LoadShader("internal/unlit");

        defaultMat = new Material();
        defaultMat->shader = defaultShader;
        defaultMat->SetRenderFlags(RF_BACK_FACE_CULLING | RF_DEPTH_TEST);
    }

    RenderWorld_Soft::~RenderWorld_Soft() {
        delete defaultMat;
    }

    void RenderWorld_Soft::RenderScene() {
        Mat4 projMat = Math::ProjectMatrix(primaryRenderView.fovY, primaryRenderView.aspect, primaryRenderView.near, primaryRenderView.far);
        Mat4 viewMat = Math::ViewMatrix(primaryRenderView.position, primaryRenderView.target, primaryRenderView.up);

        ShaderManager *shaderManager = renderer->GetShaderManager();

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
