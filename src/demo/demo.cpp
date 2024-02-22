#include "demo.h"
#include "..\platform\platform.h"
#include "..\renderer\renderer.h"
#include "..\renderer\modelmanager.h"
#include "..\math\math.h"

#include "demo_bank.h"
#include "demo_triangle.h"
#include "demo_main.h"

using namespace CG;

namespace Tutorial {
	Demo::Demo() : app(NULL), window(NULL), modelManager(NULL) {
	}

	int Demo::Run(CG::Application *app) {
		Math::Init();

		this->app = app;
		this->OnInit();

		while (!app->ShouldClosed()) {
			app->GetRenderer()->ClearColorBuffer(rgb(.0f, .0f, 0.0f));
			this->OnUpdate();
			app->GetRenderer()->SwapBuffer();

			app->PoolEvents();
		}

		this->OnShutdown();

		return 0;
	}

	void Demo::OnInit() {
		int w = 512;
		int h = 512;

		window = CreateRenderWindow(w, h, "rendering");
		app->GetRenderer()->Init(window);

		modelManager = new ModelManager();
		modelManager->Init();
	}

	void Demo::OnShutdown() {
		modelManager->Shutdown();
		delete modelManager;
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
		if (stricmp(demoName, DemoTriangle::s_name) == 0){
			demo = new DemoTriangle();
		}
		else if (stricmp(demoName, DemoMain::s_name) == 0) {
			demo = new DemoMain();
		}
		else {
			demo = new DemoBank();
		}

		return demo;
	}
}