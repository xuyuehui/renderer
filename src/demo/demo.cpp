#include "demo.h"
#include "../platform/platform.h"
#include "../renderer/renderer.h"
#include "../renderer/modelmanager.h"
#include "../math/maths.h"

#include "demo_bank.h"
#include "demo_first.h"
#include "demo_main.h"

#include "../utility/str.h"

using namespace CG;

namespace Tutorial {

Demo::Demo() : app(NULL), window(NULL) {
}

int Demo::Run(CG::Application *app) {
    Math::Init();

    this->app = app;
    this->Startup();

    while (!app->ShouldClosed()) {
        app->GetRenderer()->ClearColorBuffer(rgb(.0f, .0f, 0.0f));
        this->OnUpdate();
        app->GetRenderer()->SwapBuffer();

        app->PoolEvents();
    }

    this->OnShutdown();

    return 0;
}

void Demo::Startup() {
    int w = 800;
    int h = 600;

    window = CreateRenderWindow(w, h, "rendering");
    app->GetRenderer()->Init(window);
        
    OnInit();
}

void Demo::OnInit() {
}

void Demo::OnShutdown() {
}

Demo *InitializeDemo(int argc, char **argv) {
    const char *demoName = DemoBank::s_name;
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '+') {
            demoName = &argv[i][1];
            break;
        }
    }

    Demo *demo = NULL;
    if (Str::EqualTo(demoName, DemoFirst::s_name)){
        demo = new DemoFirst();
    }
    else if (Str::EqualTo(demoName, DemoMain::s_name)) {
        demo = new DemoMain();
    }
    else {
        demo = new DemoBank();
    }

    return demo;
}

}
