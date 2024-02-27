#ifndef SHADERMANAGER_SOFT_H
#define SHADERMANAGER_SOFT_H

#include <vector>
#include "../shadermanager.h"

using namespace std;

namespace CG {
    class Shader_Soft;

    class ShaderManager_Soft : public ShaderManager {
    public:
        ShaderManager_Soft();
        ~ShaderManager_Soft();

        void Init();
        void Shutdown();

        Shader *LoadShader(const char *filename);
    private:
        vector<Shader_Soft *> shaders;
    };
}

#endif // !SHADERMANAGER_SOFT_H
