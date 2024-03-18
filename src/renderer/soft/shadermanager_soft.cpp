#include "shadermanager_soft.h"
#include "shader_soft.h"
#include "shader_blinn_internal.h"
#include "../material.h"

namespace CG {

ShaderManager_Soft::ShaderManager_Soft() {
}

ShaderManager_Soft::~ShaderManager_Soft() {
}

void ShaderManager_Soft::Init() {
    shaders.push_back(new Blinn::UnlitShader(defaultBlinnShaderName));

    ProgramBlinn::defaultShader = LoadShader(defaultBlinnShaderName);
    ProgramBlinn::defaultMat = new BlinnMaterial();
    ProgramBlinn::defaultMat->shader = ProgramBlinn::defaultShader;
}

void ShaderManager_Soft::Shutdown() {
}

Shader * ShaderManager_Soft::LoadShader(const char *filename) {
    for (int i = 0; i < shaders.size(); i++) {
        if (strncmp(shaders[i]->ID(), filename, strlen(shaders[i]->ID())) == 0) {
            return shaders[i];
        }
    }

    return 0;
}

}
