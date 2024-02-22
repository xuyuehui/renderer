#ifndef __RENDER_ENTITY_H__
#define __RENDER_ENTITY_H__

#include "renderworld.h"
#include "..\math\matrix.h"

namespace CG {
	
	class RenderEntity {
	public:
		RenderEntity();
		const Mat4 & Transform() const;

		void UpdateRenderParams(const renderEntity_t *params);

		const renderEntity_t &RenderParams() const;
	private:
		renderEntity_t re;
		Mat4 transform;
	};

	inline const Mat4 & RenderEntity::Transform() const {
		return transform;
	}

	inline const renderEntity_t &RenderEntity::RenderParams() const {
		return re;
	}
}

#endif
