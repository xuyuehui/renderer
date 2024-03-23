#include "material.h"

namespace CG {

Material::Material() : 
    shader(NULL), doubleSided(false), enableBlend(false), alphaCutoff(false), opaque(true) {
}

BlinnMaterial::BlinnMaterial() : diffuseMap(NULL), specularMap(NULL), normalMap(NULL), emissionMap(NULL), shininess(0) {
    baseColor = Vec4(1.0f, 1.0f, 1.0f, 1.0f);
}


PbrmMaterial::PbrmMaterial() : baseColorMap(NULL), metalnessMap(NULL), roughnessMap(NULL), normalMap(NULL), occlusionMap(NULL), emissionMap(NULL), metalnessFactor(0), roughnessFactor(0) {
    baseColorFactor = Vec4(1.0f, 1.0f, 1.0f, 1.0f);
}

PbrsMaterial::PbrsMaterial() : diffuseMap(NULL), specularMap(NULL), glossinessMap(NULL), normalMap(NULL), occlusionMap(NULL), emissionMap(NULL), glossinessFactor(0) {
    diffuseFactor = Vec4(1.0f, 1.0f, 1.0f, 1.0f);
    specularFactor = Vec3(1.0f, 1.0f, 1.0f);
}

}