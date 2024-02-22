#ifndef __DEMO_H__
#define __DEMO_H__

namespace CG {
	class Application;
	class Window;
	class ModelManager;
}

namespace Tutorial {
	class Demo {
	public:
		Demo();
		virtual ~Demo() {}

		virtual int Run(CG::Application *app);
	protected:
		virtual void OnInit();
		virtual void OnShutdown();
		virtual void OnUpdate() {}
	protected:
		CG::Application *app;
		CG::Window *window;
		CG::ModelManager *modelManager;
	};

	Demo *InitializeDemo(int argc, char **argv);
}

#endif