#include "shadermanager.h"
#include "shader.h"
#include "shader_internal.h"

namespace CG {
    ShaderManager::ShaderManager() {
    }

    ShaderManager::~ShaderManager() {
    }

    void ShaderManager::Init() {
        shaders.push_back(new UnlitShader());
    }

    uint32 ShaderManager::LoadShader(const char *filename) {
        for (int i = 0; i < shaders.size(); i++) {
            if (stricmp(shaders[i]->Name(), filename) == 0) {
                return i + 1;
            }
        }

        return 0;
    }

    const Shader *ShaderManager::FindShader(uint32 shaderID) const {
        if (shaderID <= 0 || shaderID > shaders.size()) {
            return shaders[0];
        }

        return shaders[shaderID-1];
    }
}