#include "demo_triangle.h"
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
	const char *DemoTriangle::s_name = "triangle";

    DemoTriangle::DemoTriangle() : renderWorld(NULL), renderEntity(NULL), camera(NULL){
    }

    void DemoTriangle::OnInit() {
        renderWorld = app->GetRenderer()->CreateRenderWorld();

        renderEntity = new renderEntity_t();

        renderEntity->model = new RenderModelTriangle();
        
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

	void DemoTriangle::OnUpdate() {
        renderWorld->RenderScene();
	}

    void DemoTriangle::OnShutdown() {
        if (renderEntity != NULL) {
            delete renderEntity->model;
        }
        
        delete renderEntity;
        delete camera;
    }
}
