#include "model_txt.h"
#include "../shared.h"

namespace CG {
	RenderModelTxt::RenderModelTxt() {
	}

	bool RenderModelTxt::InitFromFile(const char *filename) {
		return true;
	}

	int RenderModelTxt::NumSurfaces() const {
		return 0;
	}

	modelSurface_t * RenderModelTxt::Surface(int surfaceNum) const {
		return NULL;
	}
}
