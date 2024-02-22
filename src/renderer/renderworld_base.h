#ifndef __RENDERWORLD_Base_H__
#define __RENDERWORLD_Base_H__

#include "renderworld.h"
#include "..\utility\list.h"

namespace CG {

	class RenderEntity;
	class RenderLight;
	class Renderer;

	class RenderWorld_Base : public RenderWorld {
	public:
		RenderWorld_Base();
		virtual ~RenderWorld_Base();

		virtual uint32_t AddEntityDef(const renderEntity_t &entity);
		virtual uint32_t AddLightDef(const renderLight_t &light);

		virtual void SetRenderView(const renderView_t &renderView);

		virtual void SetRenderFlags(uint32 f);
		virtual void UnsetRenderFlags(uint32 f);
		virtual bool HasRenderFlags(uint32 f) const;
		
		virtual void SetSampleType(sampleType_t st);
		virtual sampleType_t GetSampleType() const;
	protected:
		List<RenderEntity *> entities;
		List<RenderLight *> lights;

		renderView_t primaryRenderView;
		uint32 renderFlags;
		sampleType_t sampleType;
	};

	inline void RenderWorld_Base::SetRenderFlags(uint32 f) {
		renderFlags |= f;
	}

	inline void RenderWorld_Base::UnsetRenderFlags(uint32 f) {
		renderFlags &= ~(f);
	}

	inline bool RenderWorld_Base::HasRenderFlags(uint32 f) const {
		return (renderFlags & f) != 0;
	}

	inline void RenderWorld_Base::SetSampleType(sampleType_t st) {
		sampleType = st;
	}

	inline sampleType_t RenderWorld_Base::GetSampleType() const {
		return sampleType;
	}
}

#endif
