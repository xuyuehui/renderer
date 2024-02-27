#ifndef __SHADER_MANAGER_H__
#define __SHADER_MANAGER_H__

#include "../shared.h"

namespace CG {
    class Shader;

    class ShaderManager {
    public:
        virtual ~ShaderManager() {}

        virtual void Init() = 0;
        virtual void Shutdown() = 0;

        virtual Shader *LoadShader(const char *filename) = 0;
    };
}

#endif