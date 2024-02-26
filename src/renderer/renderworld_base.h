#ifndef __RENDERWORLD_Base_H__
#define __RENDERWORLD_Base_H__

#include "renderworld.h"
#include "../utility/list.h"

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
        
        virtual void SetAntiAliasingType(antiAliasingType_t aa, uint32 opt);
        virtual antiAliasingType_t GetAntiAliasingType() const;
        virtual uint32 GetAntiAliasingLevel() const;
    protected:
        List<RenderEntity *> entities;
        List<RenderLight *> lights;

        renderView_t primaryRenderView;
        uint32 renderFlags;
        uint32 antiAlias;
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

    inline void RenderWorld_Base::SetAntiAliasingType(antiAliasingType_t aa, uint32 opt) {
        antiAlias = (aa << 16) | opt;
    }

    inline antiAliasingType_t RenderWorld_Base::GetAntiAliasingType() const {
        return (antiAliasingType_t)((antiAlias >> 16) & 0x00000FFFF);
    }

    inline uint32 RenderWorld_Base::GetAntiAliasingLevel() const {
        return antiAlias & 0x00000FFFF;
    }
}

#endif
