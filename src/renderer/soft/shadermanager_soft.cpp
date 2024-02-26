#include "shadermanager_soft.h"
#include "shader.h"
#include "shader_internal.h"

namespace CG {
    ShaderManager_Soft::ShaderManager_Soft() {
    }

    ShaderManager_Soft::~ShaderManager_Soft() {
    }

    void ShaderManager_Soft::Init() {
        shaders.push_back(new UnlitShader());
    }

    void ShaderManager_Soft::Shutdown() {
    }

    uint32 ShaderManager_Soft::LoadShader(const char *filename) {
        for (int i = 0; i < shaders.size(); i++) {
            if (stricmp(shaders[i]->Name(), filename) == 0) {
                return i + 1;
            }
        }

        return 0;
    }

    const Shader *ShaderManager_Soft::FindShader(uint32 shaderID) const {
        if (shaderID <= 0 || shaderID > shaders.size()) {
            return shaders[0];
        }

        return shaders[shaderID - 1];
    }
}