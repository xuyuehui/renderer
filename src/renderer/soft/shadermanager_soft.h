#ifndef SHADERMANAGER_SOFT_H
#define SHADERMANAGER_SOFT_H

#include <vector>
#include "../shadermanager.h"

using namespace std;

namespace CG {
    class ShaderManager_Soft : public ShaderManager {
    public:
        ShaderManager_Soft();
        ~ShaderManager_Soft();

        void Init();
        void Shutdown();

        uint32 LoadShader(const char *filename);
        const Shader *FindShader(uint32 shaderID) const;
    private:
        vector<Shader *> shaders;
    };
}

#endif // !SHADERMANAGER_SOFT_H
