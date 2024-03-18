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

    bool doubleSided;
    bool enableBlend;
    bool alphaCutoff;
public:
    Material();
    virtual ~Material() {}
};

class BlinnMaterial : public Material {
public:
    Texture *diffuseMap;
    Texture *normalMap;
    Texture *specularMap;
    Texture *emissionMap;

    Vec4 baseColor;
    float shininess;
public:
    BlinnMaterial();
};

class PbrmMaterial : public Material {
public:
    Vec4 baseColorFactor;
    float metalnessFactor;
    float roughnessFactor;

    Texture *baseColorMap;
    Texture *metalnessMap;
    Texture *roughnessMap;
    Texture *normalMap;
    Texture *occlusionMap;
    Texture *emissionMap;
public:
    PbrmMaterial();
};

class PbrsMaterial : public Material {
public:
    Vec4 diffuseFactor;
    Vec3 specularFactor;
    float glossinessFactor;

    Texture *diffuseMap;
    Texture *specularMap;
    Texture *glossinessMap;
    Texture *normalMap;
    Texture *occlusionMap;
    Texture *emissionMap;
public:
    PbrsMaterial();
};

}

#endif