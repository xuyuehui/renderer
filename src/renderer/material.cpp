#include "material.h"

namespace CG {
    Material::Material() : shader(0), flags(0), albedo(NULL), diffuse(NULL), specular(NULL), normal(NULL) {
    }
}