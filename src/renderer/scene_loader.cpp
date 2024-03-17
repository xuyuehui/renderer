#include "scene_loader.h"
#include "renderer.h"
#include "renderworld.h"
#include "renderer.h"
#include "../utility/str.h"
#include "model.h"
#include "modelmanager.h"
#include "material.h"
#include "texturemanager.h"
#include "shadermanager.h"

using namespace std;

namespace CG {

renderScene_s::~renderScene_s() {
    delete light;
    for (int i = 0; i < entities.size(); i++) {
        delete entities[i];
    }
}
static inline const char *WrapPath(const char *path) {
    if (Str::EqualTo(path, "null")) {
        return NULL;
    }

    return path;
}

static inline bool WrapKnob(const char *str) {
    if (Str::EqualTo(str, "on")) {
        return true;
    }

    assert(Str::EqualTo(str, "off"));

    return false;
}

typedef struct {
    Vec3 background;
    char environment[MAX_BUFFER_SIZE];
    char skybox[MAX_BUFFER_SIZE];
    char shadow[MAX_BUFFER_SIZE];
    float ambient;
    float punctual;
}sceneLight_t;

typedef struct {
    int index;
    Vec4 baseColor;
    float shininess;
    char diffuseMap[MAX_BUFFER_SIZE];
    char specularMap[MAX_BUFFER_SIZE];
    char emissionMap[MAX_BUFFER_SIZE];
    char doubleSided[MAX_BUFFER_SIZE];
    char enableBlend[MAX_BUFFER_SIZE];
    float alphaCutoff;
}sceneBlinn_t;

typedef struct {
    int index;
    Vec4 baseColorFactor;
    float metalnessFactor;
    float roughnessFactor;
    char basecolorMap[MAX_BUFFER_SIZE];
    char metalnessMap[MAX_BUFFER_SIZE];
    char roughnessMap[MAX_BUFFER_SIZE];
    char normalMap[MAX_BUFFER_SIZE];
    char occlusionMap[MAX_BUFFER_SIZE];
    char emissionMap[MAX_BUFFER_SIZE];
    char doubleSided[MAX_BUFFER_SIZE];
    char enableBlend[MAX_BUFFER_SIZE];
    float alphaCutoff;
}scenePbrm_t;

typedef struct {
    int index;
    Vec4 diffuseFactor;
    Vec3 specularFactor;
    float glossinessFactor;
    char diffuseMap[MAX_BUFFER_SIZE];
    char specularMap[MAX_BUFFER_SIZE];
    char glossinessMap[MAX_BUFFER_SIZE];
    char normalMap[MAX_BUFFER_SIZE];
    char occlusionMap[MAX_BUFFER_SIZE];
    char emissionMap[MAX_BUFFER_SIZE];
    char doubleSided[MAX_BUFFER_SIZE];
    char enableBlend[MAX_BUFFER_SIZE];
    float alphaCutoff;
}scenePbrs_t;

typedef struct {
    int index;
    Mat4 matrix;
}sceneTransform_t;

typedef struct {
    int index;
    char mesh[MAX_BUFFER_SIZE];
    char skeleton[MAX_BUFFER_SIZE];
    int attached;
    int material;
    int transform;
}sceneModel_t;

static sceneLight_t ReadLight(FILE *fp) {
    sceneLight_t light;
    char header[MAX_BUFFER_SIZE];
    int items = 0;

    items = fscanf(fp, " %s", header);
    assert(Str::EqualTo(header, "lighting:"));

    items = fscanf(fp, " background: %f %f %f", &light.background.x, &light.background.y, &light.background.z);
    assert(items == 3);
    items = fscanf(fp, " environment: %s", light.environment);
    assert(items == 1);
    items = fscanf(fp, " skybox: %s", light.skybox);
    assert(items == 1);
    items = fscanf(fp, " shadow: %s", light.shadow);
    assert(items == 1);
    items = fscanf(fp, " ambient: %f", &light.ambient);
    assert(items == 1);
    items = fscanf(fp, " punctual: %f", &light.punctual);
    assert(items == 1);

    return light;
}

static sceneBlinn_t readBlinnMaterial(FILE *fp) {
    sceneBlinn_t material;
    int items;

    const char *prefix = "assets/";
    char buffer[MAX_BUFFER_SIZE];

    items = fscanf(fp, " material %d:", &material.index);
    assert(items == 1);
    items = fscanf(fp, " basecolor: %f %f %f %f", &material.baseColor.x, &material.baseColor.y, &material.baseColor.z, &material.baseColor.w);
    assert(items == 4);
    items = fscanf(fp, " shininess: %f", &material.shininess);
    assert(items == 1);
    items = fscanf(fp, " diffuse_map: %s", buffer);
    assert(items == 1);
    Str::Combin(material.diffuseMap, MAX_BUFFER_SIZE, WrapPath(buffer) ? prefix : "", buffer);

    items = fscanf(fp, " specular_map: %s", buffer);
    assert(items == 1);
    Str::Combin(material.specularMap, MAX_BUFFER_SIZE, WrapPath(buffer) ? prefix : "", buffer);

    items = fscanf(fp, " emission_map: %s", buffer);
    assert(items == 1);
    Str::Combin(material.emissionMap, MAX_BUFFER_SIZE, WrapPath(buffer) ? prefix : "", buffer);

    items = fscanf(fp, " double_sided: %s", material.doubleSided);
    assert(items == 1);
    items = fscanf(fp, " enable_blend: %s", material.enableBlend);
    assert(items == 1);
    items = fscanf(fp, " alpha_cutoff: %f", &material.alphaCutoff);
    assert(items == 1);

    return material;
}

static std::vector<sceneBlinn_t> readBlinnMaterials(FILE *file) {
    std::vector<sceneBlinn_t> materials;
    int numMaterials;
    int items;
    int i;

    items = fscanf(file, " materials %d:", &numMaterials);
    assert(items == 1);

    materials.reserve(numMaterials);

    for (i = 0; i < numMaterials; i++) {
        sceneBlinn_t material = readBlinnMaterial(file);
        assert(material.index == i);
        materials.push_back(material);
    }

    return materials;
}

static sceneTransform_t readTransform(FILE *file) {
    sceneTransform_t transform;
    int items;
    int i;

    items = fscanf(file, " transform %d:", &transform.index);
    assert(items == 1);
    for (i = 0; i < 4; i++) {
        items = fscanf(file, " %f %f %f %f", &transform.matrix[i][0], &transform.matrix[i][1], &transform.matrix[i][2], &transform.matrix[i][3]);
        assert(items == 4);
    }

    return transform;
}

static std::vector<sceneTransform_t> readTransforms(FILE *file) {
    std::vector<sceneTransform_t> transforms;
    int numTransforms;
    int items;
    int i;

    items = fscanf(file, " transforms %d:", &numTransforms);
    assert(items == 1);

    for (i = 0; i < numTransforms; i++) {
        sceneTransform_t transform = readTransform(file);
        assert(transform.index == i);
        transforms.push_back(transform);
    }
    return transforms;
}

static sceneModel_t readModel(FILE *file) {
    sceneModel_t model;
    int items;
    char buffer[MAX_BUFFER_SIZE];
    const char *prefix = "assets/";

    items = fscanf(file, " model %d:", &model.index);
    assert(items == 1);
    items = fscanf(file, " mesh: %s", buffer);
    assert(items == 1);
    Str::Combin(model.mesh, MAX_BUFFER_SIZE, WrapPath(buffer) ? prefix : "", buffer);
    items = fscanf(file, " skeleton: %s", model.skeleton);
    assert(items == 1);
    Str::Combin(model.skeleton, MAX_BUFFER_SIZE, WrapPath(buffer) ? prefix : "", buffer);
    items = fscanf(file, " attached: %d", &model.attached);
    assert(items == 1);
    items = fscanf(file, " material: %d", &model.material);
    assert(items == 1);
    items = fscanf(file, " transform: %d", &model.transform);
    assert(items == 1);

    return model;
}

static std::vector<sceneModel_t> readModels(FILE *file) {
    std::vector<sceneModel_t> models;
    int numModels;
    int items;
    int i;

    items = fscanf(file, " models %d:", &numModels);
    assert(items == 1);
    for (i = 0; i < numModels; i++) {
        sceneModel_t model = readModel(file);
        assert(model.index == i);
        models.push_back(model);
    }

    return models;
}

void CreateBinnScene(Renderer *renderer, FILE *file, renderScene_t *scene, const Mat4 &root) {
    scene->shadingMode = SHADING_BLINN;

    sceneLight_t light = ReadLight(file);
    std::vector<sceneBlinn_t> materials = readBlinnMaterials(file);
    std::vector<sceneTransform_t> transforms = readTransforms(file);
    std::vector<sceneModel_t> models = readModels(file);

    scene->light = new renderLight_t();
    scene->light->ambient = light.ambient;
    scene->light->punctual = light.punctual;

    const char *prefix = "assets/";

    for (int i = 0; i < models.size(); i++) {
        sceneModel_t &modelDecl = models[i];

        const char *mesh = WrapPath(modelDecl.mesh);
        const char *skeleon = WrapPath(modelDecl.skeleton);

        Mat4 transform = root * transforms[modelDecl.transform].matrix;
        sceneBlinn_t &materialDecl = materials[modelDecl.material];

        Material *material = new Material();

        material->shader = renderer->GetShaderManager()->LoadShader("internal/unlit");

        material->baseColor = materialDecl.baseColor;
        material->shininess = materialDecl.shininess;

        material->diffuse = renderer->GetTextureManager()->LoadTexture(WrapPath(materialDecl.diffuseMap));
        material->specular = renderer->GetTextureManager()->LoadTexture(WrapPath(materialDecl.specularMap));
        material->emission = renderer->GetTextureManager()->LoadTexture(WrapPath(materialDecl.emissionMap));

        material->doubleSided = WrapKnob(materialDecl.doubleSided);
        material->enableBlend = WrapKnob(materialDecl.enableBlend);
        material->alphaCutoff = materialDecl.alphaCutoff;

        RenderModel *model = renderer->GetModelManager()->LoadModel(mesh);
        if (model) {
            model->SetMaterial(0, material);

            renderEntity_t *entity = new renderEntity_t();
            entity->orginal = transform;
            entity->position = Vec3(0.0f, 0.0f, 0.0f);
            entity->rotation = Quat::Indentity();
            entity->scale = Vec3(1.0f, 1.0f, 1.0f);

            entity->model = model;
            scene->entities.push_back(entity);
        }
    }
}

renderScene_t *SceneLoader::LoadScene(Renderer *renderer, const char *filename, const Mat4 &root) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        return NULL;
    }
    
    char sceneType[MAX_BUFFER_SIZE];

    int num = fscanf(file, " type: %s", sceneType);
    assert(num == 1);

    renderScene_t *scene = new renderScene_t();
    
    if (Str::EqualTo(sceneType, "blinn")) {
        CreateBinnScene(renderer, file, scene, root);
    }
    else if (Str::EqualTo(sceneType, "pbrm")) {
        scene->shadingMode = SHADING_PBR;
        sceneLight_t light = ReadLight(file);
    }
    else if (Str::EqualTo(sceneType, "pbrs")) {
        scene->shadingMode = SHADING_PBR;
    }
    else {
        assert(false);
    }

    fclose(file);

    return scene;
}

}