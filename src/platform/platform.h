#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#include "..\shared.h"

namespace CG {
	class Renderer;
	class Window;

	typedef void (*keyboardCallback_t) (uint32_t, bool);

	class Application {
	public:
		virtual ~Application() {}
		
		virtual bool ShouldClosed() = 0;
		virtual void PoolEvents() = 0;
		virtual void Exit() = 0;

		virtual Renderer *GetRenderer() = 0;
	};

	class Window {
	public:
		virtual ~Window() {}

		virtual void SetKeyboardCallback(keyboardCallback_t callback) = 0;
		virtual void *GetHandle() const = 0;
		virtual void GetSize(int &width, int &height) const = 0;
	};

	Application * InitializeApplication(int argc, char **argv);
	Window * CreateRenderWindow(int width, int height, const char *title);
}

#endif