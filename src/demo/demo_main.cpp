#include "demo_main.h"
#include "../platform/platform.h"
#include "../renderer/renderer.h"
#include "../math/maths.h"
#include "../shared.h"
#include "../renderer/modelmanager.h"
#include "../renderer/model.h"
#include "../renderer/renderworld.h"
#include "../math/vector.h"

namespace Tutorial {
    const char *DemoMain::s_name = "main";

    DemoMain::DemoMain() : renderWorld(NULL), renderEntity(NULL), camera(NULL) {
    }

    void DemoMain::OnInit() {
        Demo::OnInit();

        renderWorld = app->GetRenderer()->CreateRenderWorld();

        renderEntity = new CG::renderEntity_t();

        renderEntity->model = app->GetRenderer()->GetModelManager()->GetModel("./assets/teapot_high.txt");
        renderEntity->position = Vec3(0.0f, 0.0f, 10.0f);
        renderEntity->rotation = Quat::Indentity();
        renderEntity->scale = Vec3(1.0f, 1.0f, 1.0f);

        renderWorld->AddEntityDef(*renderEntity);

        camera = new renderView_t();

        camera->position = Vec3(.0f, .0f, -3.0f);
        camera->target = Vec3(.0f, .0f, .0f);
        camera->fovY = PI / 3;
        camera->aspect = 1.0f;
        camera->near = 0.1f;
        camera->far = 1000.0f;
        camera->up = Vec3(.0f, 1.0f, 0.0f);

        renderWorld->SetRenderView(*camera);
    }

    void DemoMain::OnUpdate() {
        renderWorld->RenderScene();
    }

    void DemoMain::OnShutdown() {
        delete renderEntity;
        delete renderWorld;
        delete camera;
        Demo::OnShutdown();
    }
}
