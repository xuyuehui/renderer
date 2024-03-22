#include "shadermanager_soft.h"
#include "shader_soft.h"
#include "shader_lib_blinn.h"
#include "shader_lib_pbrm.h"
#include "shader_lib_pbrs.h"
#include "../material.h"

namespace CG {

ShaderManager_Soft::ShaderManager_Soft() {
}

ShaderManager_Soft::~ShaderManager_Soft() {
}

void ShaderManager_Soft::Init() {
    shaders.insert(make_pair(defaultBlinnShaderName, new Blinn::CommonShader()));
    shaders.insert(make_pair(defaultPbrmShaderName, new Pbrm::CommonShader()));
    shaders.insert(make_pair(defaultPbrsShaderName, new Pbrs::CommonShader()));

    Blinn::Program::defaultShader = LoadShader(defaultBlinnShaderName);
    Blinn::Program::defaultMat = new BlinnMaterial();
    Blinn::Program::defaultMat->shader = Blinn::Program::defaultShader;

    Pbrm::Program::defaultShader = LoadShader(defaultPbrmShaderName);
    Pbrm::Program::defaultMat = new PbrmMaterial();
    Pbrm::Program::defaultMat->shader = Pbrm::Program::defaultShader;

    Pbrs::Program::defaultShader = LoadShader(defaultPbrsShaderName);
    Pbrs::Program::defaultMat = new PbrsMaterial();
    Pbrs::Program::defaultMat->shader = Pbrs::Program::defaultShader;
}

void ShaderManager_Soft::Shutdown() {
}

Shader * ShaderManager_Soft::LoadShader(const char *filename) {
    if (shaders.contains(filename)) {
        return shaders[filename];
    }

    return NULL;
}

}
