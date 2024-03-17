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
#include "../math/angles.h"

namespace Tutorial {

const char *DemoMain::s_name = "main";

DemoMain::DemoMain() : renderWorld(NULL), camera(NULL), scene(NULL) {
}

void DemoMain::OnInit() {
    Mat4 translation = Mat4::FromTranslate(0, 0, -1.369f);
    Mat4 rotation = Mat4::FromRotateX(DEG2RAD(-90));
    Mat4 scale = Mat4::FromScale(0.331f, 0.331f, 0.331f);
    Mat4 root = scale * rotation * translation;
    scene = SceneLoader::LoadScene(app->GetRenderer(), "assets/vivi/vivi.scn", root);
    renderWorld = app->GetRenderer()->CreateRenderWorld(SHADING_BLINN);

    int width, height;
    window->GetSize(width, height);
    camera = new renderView_t();

    camera->position = Vec3(.0f, .0f, 1.5f);
    camera->target = Vec3(.0f, .0f, .0f);
    camera->fovY = Math::PI / 3;
    camera->aspect = (float)width / height;
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
