#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include "../shared.h"

namespace CG {
    class Texture;
    class Shader;

    class Material {
    public:
        Shader *shader;
        Texture *albedo;
        Texture *diffuse;
        Texture *specular;
        Texture *normal;
    public:
        Material();

        void SetRenderFlags(uint32 f);
        void UnseRenderFlags(uint32 f);
        bool HasRenderFlags(uint32 f) const;

    protected:
        uint32 flags;
    };

    inline void Material::SetRenderFlags(uint32 f) {
        flags |= f;
    }

    inline void Material::UnseRenderFlags(uint32 f) {
        flags &= ~(f);
    }

    inline bool Material::HasRenderFlags(uint32 f) const {
        return (flags & f) != 0;
    }
}

#endif