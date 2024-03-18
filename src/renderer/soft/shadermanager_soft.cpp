#include "shadermanager_soft.h"
#include "shader_soft.h"
#include "shader_blinn_internal.h"
#include "shader_pbrm_internal.h"
#include "shader_pbrs_internal.h"
#include "../material.h"

namespace CG {

ShaderManager_Soft::ShaderManager_Soft() {
}

ShaderManager_Soft::~ShaderManager_Soft() {
}

void ShaderManager_Soft::Init() {
    shaders.insert(make_pair(defaultBlinnShaderName, new Blinn::UnlitShader()));
    shaders.insert(make_pair(defaultPbrmShaderName, new Pbrm::UnlitShader()));
    shaders.insert(make_pair(defaultPbrsShaderName, new Pbrs::UnlitShader()));

    ProgramBlinn::defaultShader = LoadShader(defaultBlinnShaderName);
    ProgramBlinn::defaultMat = new BlinnMaterial();
    ProgramBlinn::defaultMat->shader = ProgramBlinn::defaultShader;

    ProgramPbrm::defaultShader = LoadShader(defaultPbrmShaderName);
    ProgramPbrm::defaultMat = new PbrmMaterial();
    ProgramPbrm::defaultMat->shader = ProgramPbrm::defaultShader;

    ProgramPbrs::defaultShader = LoadShader(defaultPbrsShaderName);
    ProgramPbrs::defaultMat = new PbrsMaterial();
    ProgramPbrs::defaultMat->shader = ProgramPbrs::defaultShader;
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
