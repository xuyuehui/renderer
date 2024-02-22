#include "renderworld_soft.h"
#include "../../math/math.h"
#include "../render_entity.h"
#include "../model.h"
#include "../material.h"

#include "soft_renderer.h"
#include "shadermanager.h"
#include "shader.h"
#include "msaa.h"

namespace CG {

    static inline vdata_t VDATA(const srfTriangles_t *triangles, int vertIndex, const RenderEntity *entity) {
        vdata_t vd;

        vd.localMat = entity->Transform();
        vd.position = triangles->verts[vertIndex].xyz;
        vd.normal = triangles->verts[vertIndex].normal;
        vd.texcoord = triangles->verts[vertIndex].st;
        vd.color = triangles->verts[vertIndex].color;
        vd.tangent = triangles->verts[vertIndex].tangent;

        return vd;
    }

    static inline void PerspectiveDivision(Vec4 &v) {
        v.w = 1.0f / v.w;
        v.x *= v.w;
        v.y *= v.w;
        v.z *= v.w;
    }

    static inline int ScreenMapping_X(float x, int width) {
        return floor((x * 0.5f + 0.5f) * width);
    }

    static inline int ScreenMapping_Y(float y, int height) {
        return floor((y * 0.5f + 0.5f) * height);
    }

    RenderWorld_Soft::RenderWorld_Soft(Renderer *renderer) {
        this->renderer = dynamic_cast<SoftRenderer*>(renderer);

        defaultMat = new Material();
        defaultMat->SetRenderFlags(RF_BACK_FACE_CULLING);
    }

    RenderWorld_Soft::~RenderWorld_Soft() {
        delete defaultMat;
    }

    void RenderWorld_Soft::RenderScene() {
        renderer->ClearDepthBuffer(1.0f);

        Mat4 projMat = Math::ProjectMatrix(primaryRenderView.fovY, primaryRenderView.aspect, primaryRenderView.near, primaryRenderView.far);
        Mat4 viewMat = Math::ViewMatrix(primaryRenderView.position, primaryRenderView.target, primaryRenderView.up);
        Mat4 vp = projMat * viewMat;

        ShaderManager *shaderManager = renderer->shaderManager;

        renderTargetDesc_t rtd;
        renderer->GetColorBufferDesc(rtd);

        for (int i = 0; i < entities.Num(); i++) {
            const RenderEntity *entity = entities[i];
            Mat4 mvpMat = vp * entity->Transform();
            Mat4 modelInvTransport = entity->Transform().Inverse().Transport();

            RenderModel *model = entity->RenderParams().model;

            for (int i = 0; i < model->NumSurfaces(); i++) {
                modelSurface_t *surface = model->Surface(i);
                const Material *material = surface->material != NULL ? surface->material : defaultMat;
                const Shader *shader = shaderManager->FindShader(material->shaderID);

                for (int i = 0, tidx = 0; i < surface->geometry->numIndexes; i += 3, tidx++) {

                    // Vertex Processing
                    v2f_t v0 = shader->Vertex(VDATA(surface->geometry, i, entity));
                    v2f_t v1 = shader->Vertex(VDATA(surface->geometry, i + 1, entity));
                    v2f_t v2 = shader->Vertex(VDATA(surface->geometry, i + 2, entity));

                    // Perspective Division
                    PerspectiveDivision(v0.position);
                    PerspectiveDivision(v1.position);
                    PerspectiveDivision(v2.position);

                    // Screen Clipping
                    if ((v0.position.x < -1.0f && v1.position.x < -1.0f && v2.position.x < -1.0f) ||
                        (v0.position.x >  1.0f && v1.position.x >  1.0f && v2.position.x >  1.0f) ||
                        (v0.position.y < -1.0f && v1.position.y < -1.0f && v2.position.y < -1.0f) ||
                        (v0.position.y >  1.0f && v1.position.y >  1.0f && v2.position.y >  1.0f) ||
                        (v0.position.z < -1.0f && v1.position.z < -1.0f && v2.position.z < -1.0f) ||
                        (v0.position.z >  1.0f && v1.position.z >  1.0f && v2.position.z >  1.0f)){
                        continue;
                    }

                    // Back Face Culling
                    if (this->HasRenderFlags(RF_WIREFRAME_MODE) || material->HasRenderFlags(RF_WIREFRAME_MODE | RF_BACK_FACE_CULLING)) {
                        const Vec3 &e1 = (v1.position - v0.position).ToVec3();
                        const Vec3 &e2 = (v2.position - v0.position).ToVec3();

                        if (e1.Cross(e2).z < 0.0f) {
                            continue;
                        }
                    }
                    
                    v0.position.x = ScreenMapping_X(v0.position.x, rtd.width);
                    v1.position.x = ScreenMapping_X(v1.position.x, rtd.width);
                    v2.position.x = ScreenMapping_X(v2.position.x, rtd.width);
                    v0.position.y = ScreenMapping_Y(v0.position.y, rtd.height);
                    v1.position.y = ScreenMapping_Y(v1.position.y, rtd.height);
                    v2.position.y = ScreenMapping_Y(v2.position.y, rtd.height);

                    v0.position.z = 1.0f / v0.position.z;
                    v1.position.z = 1.0f / v1.position.z;
                    v2.position.z = 1.0f / v2.position.z;

                    // wireframe mode
                    if (this->HasRenderFlags(RF_WIREFRAME_MODE)) {
                        renderer->DrawLine(v0.position.ToVec2(), v1.position.ToVec2(), COLOR_WHITE, 0.0f);
                        renderer->DrawLine(v1.position.ToVec2(), v2.position.ToVec2(), COLOR_WHITE, 0.0f);
                        renderer->DrawLine(v2.position.ToVec2(), v0.position.ToVec2(), COLOR_WHITE, 0.0f);

                        continue;
                    }

                    // reference : https://www.scratchapixel.com/lessons/3d-basic-rendering/rasterization-practical-implementation/overview-rasterization-algorithm.html
                    const int xMin = max(min(v0.position.x, min(v1.position.x, v2.position.x)), 0);
                    const int xMax = min(max(v0.position.x, max(v1.position.x, v2.position.x)), rtd.width - 1);
                    const int yMin = max(min(v0.position.y, min(v1.position.y, v2.position.y)), 0);
                    const int yMax = min(max(v0.position.y, max(v1.position.y, v2.position.y)), rtd.height - 1);

                    float area = Math::EdgeFunction(v0.position.ToVec3(), v1.position.ToVec3(), v2.position.ToVec3());
                    unsigned short mask = 0;

                    for (int x = xMin; x < xMax; x++) {
                        for (int y = yMin; y < yMax; y++) {
                            Vec3 pos(x + 0.5f, y + 0.5f, 1.0f);
                            float w0, w1, w2;

                            if (GetSampleType() > SAMPLE_DEFAULT) {
                                GetMSAAMask(GetSampleType(), mask, v0.position.ToVec3(), v1.position.ToVec3(), v2.position.ToVec3(), pos);

                                if (mask == 0) {
                                    continue;
                                }

                                Math::OutsideTest(v0.position.ToVec3(), v1.position.ToVec3(), v2.position.ToVec3(), pos, w0, w1, w2);
                            }
                            else if (Math::OutsideTest(v0.position.ToVec3(), v1.position.ToVec3(), v2.position.ToVec3(), pos, w0, w1, w2)){
                                continue;
                            }

                            w0 /= area;
                            w1 /= area;
                            w2 /= area;

                            float denom = (w0 * v0.position.z + w1 * v1.position.z + w2 * v2.position.z);
                            pos.z = 1.0f / denom;
                            if (isnan(pos.z)) {
                                continue;
                            }
                        }
                    }
                }
            }
        }
    }
}
