#include "material.h"

namespace CG {
    Material::Material() : shaderID(0), flags(0), albedo(NULL), diffuse(NULL), specular(NULL), normal(NULL) {
    }
}