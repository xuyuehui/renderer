#include "demo_first.h"
#include "../platform/platform.h"
#include "../renderer/renderer.h"
#include "../math/maths.h"
#include "../shared.h"
#include "../utility/utility.h"
#include "../renderer/modelmanager.h"
#include "../renderer/model.h"
#include "../renderer/renderworld.h"
#include "../math/vector.h"
#include "../renderer/model_triangle.h"

using namespace CG;

namespace Tutorial {

const char *DemoFirst::s_name = "first";

DemoFirst::DemoFirst() : renderWorld(NULL), renderEntity(NULL), camera(NULL){
}

void DemoFirst::OnInit() {
    renderWorld = app->GetRenderer()->CreateRenderWorld(SHADING_BLINN);

    int width, height;
    window->GetSize(width, height);

    renderEntity = new CG::renderEntity_t();

    renderEntity->model = app->GetRenderer()->GetModelManager()->LoadModel("./assets/cube.txt");
    renderEntity->orginal.Indentity();
    renderEntity->position = Vec3(0.0f, 0.0f, 10.0f);
    renderEntity->rotation = Quat::Indentity();
    renderEntity->scale = Vec3(1.0f, 1.0f, 1.0f);

    renderWorld->AddEntityDef(*renderEntity);

    camera = new renderView_t();

    camera->position = Vec3(.0f, .0f, -3.0f);
    camera->target = Vec3(.0f, .0f, .0f);
    camera->fovY = Math::PI / 3;
    camera->aspect = (float)width / height;
    camera->near = 0.1f;
    camera->far = 1000.0f;
    camera->up = Vec3(.0f, 1.0f, 0.0f);

    renderWorld->SetRenderView(*camera);
}

void DemoFirst::OnUpdate() {
    app->GetRenderer()->ClearColorBuffer(Vec3(.0f, .0f, .0f));
    app->GetRenderer()->ClearDepthBuffer(1.0f);
    renderWorld->RenderScene();
}

void DemoFirst::OnShutdown() {
    delete renderEntity;
    delete renderWorld;
    delete camera;
    Demo::OnShutdown();
}

}
