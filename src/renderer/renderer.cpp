#include "renderer.h"
#include "soft/soft_renderer.h"

namespace CG {
	Renderer *CreateRendererAPI(rendererAPIType_t type) {
		return new SoftRenderer();
	}
}