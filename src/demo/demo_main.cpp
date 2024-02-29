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

    DemoMain::DemoMain() : renderWorld(NULL), renderEntity(NULL) {
    }

    void DemoMain::OnInit() {
        Demo::OnInit();

        renderWorld = app->GetRenderer()->CreateRenderWorld();

        renderEntity = new CG::renderEntity_t();

        renderEntity->model = app->GetRenderer()->GetModelManager()->GetModel("./assets/cube.txt");
        renderEntity->position = Vec3(0.0f, 0.0f, 10.0f);
        renderEntity->rotation = Quat::Indentity();
        renderEntity->scale = Vec3(1.0f, 1.0f, 1.0f);

        renderWorld->AddEntityDef(*renderEntity);
    }

    void DemoMain::OnUpdate() {
        renderWorld->RenderScene();

        app->GetRenderer()->SwapBuffer();
    }

    void DemoMain::OnShutdown() {
        delete renderEntity;
        delete renderWorld;
        Demo::OnShutdown();
    }
}