#include "material.h"

namespace CG {
    Material::Material() : 
        shader(NULL), diffuse(NULL), specular(NULL), normal(NULL), emission(NULL), 
        doubleSided(false), enableBlend(false), alphaCutoff(false), shininess(0) {
        
        baseColor = Vec4(1.0f, 1.0f, 1.0f, 1.0f);
    }
}