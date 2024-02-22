#ifndef __MODEL_TXT_H__
#define __MODEL_TXT_H__

#include "model.h"

namespace CG {
	class RenderModelTxt : public RenderModel {
	public:
		RenderModelTxt();

		bool InitFromFile(const char *filename);

		int NumSurfaces() const;

		modelSurface_t *Surface(int surfaceNum) const;
	};
}

#endif