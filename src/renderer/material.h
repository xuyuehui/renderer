#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include "../shared.h"
#include "../math/vector.h"

namespace CG {
    class Texture;
    class Shader;

    class Material {
    public:
        Shader *shader;

        Texture *diffuse;
        Texture *normal;
        Texture *specular;
        Texture *emission;

        bool doubleSided;
        bool enableBlend;
        bool alphaCutoff;

        Vec4 baseColor;
        float shininess;
    public:
        Material();
    };
}

#endif