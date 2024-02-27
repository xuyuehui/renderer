#include "shadermanager_soft.h"
#include "shader_soft.h"
#include "shader_internal.h"

namespace CG {
    ShaderManager_Soft::ShaderManager_Soft() {
    }

    ShaderManager_Soft::~ShaderManager_Soft() {
    }

    void ShaderManager_Soft::Init() {
        shaders.push_back(new UnlitShader("internal/unlit"));
    }

    void ShaderManager_Soft::Shutdown() {
    }

    Shader * ShaderManager_Soft::LoadShader(const char *filename) {
        for (int i = 0; i < shaders.size(); i++) {
            if (stricmp(shaders[i]->ID(), filename) == 0) {
                return shaders[i];
            }
        }

        return 0;
    }
}