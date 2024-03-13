#include "demo_main.h"
#include "../platform/platform.h"
#include "../renderer/renderer.h"
#include "../math/maths.h"
#include "../shared.h"
#include "../renderer/modelmanager.h"
#include "../renderer/model.h"
#include "../renderer/renderworld.h"
#include "../math/vector.h"
#include "../renderer/scene_loader.h"

namespace Tutorial {

const char *DemoMain::s_name = "main";

DemoMain::DemoMain() : renderWorld(NULL), camera(NULL), scene(NULL) {
}

void DemoMain::OnInit() {
    Mat4 mat = Math::FromRTS(Vec3(.0f, 0.0f, 10.0f), Quat::Indentity(), Vec3(1.0f, 1.0f, 1.0f));
    scene = SceneLoader::LoadScene(app->GetRenderer(), "assets/azura/azura.scn", mat);
    renderWorld = app->GetRenderer()->CreateRenderWorld(SHADING_BLINN);

    camera = new renderView_t();

    camera->position = Vec3(.0f, .0f, -3.0f);
    camera->target = Vec3(.0f, .0f, .0f);
    camera->fovY = PI / 3;
    camera->aspect = 1.0f;
    camera->near = 0.1f;
    camera->far = 1000.0f;
    camera->up = Vec3(.0f, 1.0f, 0.0f);

    renderWorld->SetRenderView(*camera);

    for (int i = 0; i < scene->entities.size(); i++) {
        renderWorld->AddEntityDef(*scene->entities[i]);
    }
}

void DemoMain::OnUpdate() {
    renderWorld->RenderScene();
}

void DemoMain::OnShutdown() {
    delete renderWorld;
    delete camera;
    delete scene;
    Demo::OnShutdown();
}

}
