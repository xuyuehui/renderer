#include "demo_main.h"
#include "../platform/platform.h"
#include "../renderer/renderer.h"
#include "../math/math.h"
#include "../shared.h"
#include "../renderer/modelmanager.h"
#include "../renderer/model.h"

namespace Tutorial {
    const char *DemoMain::s_name = "main";

    DemoMain::DemoMain() : model(NULL) {
    }

    void DemoMain::OnInit() {
        Demo::OnInit();
        model = app->GetRenderer()->GetModelManager()->GetModel("./assets/cube.txt");
    }

    void DemoMain::OnUpdate() {
    }

    void DemoMain::OnShutdown() {
        delete model;
        Demo::OnShutdown();
    }
}