#ifndef __SHADER_MANAGER_H__
#define __SHADER_MANAGER_H__

#include "../../shared.h"
#include <vector>

using namespace std;

namespace CG {
    class Shader;

    class ShaderManager {
    public:
        ShaderManager();
        ~ShaderManager();

        void Init();

        uint32 LoadShader(const char *filename);
        const Shader *FindShader(uint32 shaderID) const;
    private:
        vector<Shader *> shaders;
    };
}

#endif