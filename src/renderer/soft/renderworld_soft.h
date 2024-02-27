#ifndef __RENDERWORLD_SOFT_H__
#define __RENDERWORLD_SOFT_H__

#include "../renderworld_base.h"

namespace CG {
    class Renderer;
    class SoftRenderer;
    class Material;
    class Shader;

    class RenderWorld_Soft : public RenderWorld_Base {
    public:
        RenderWorld_Soft(Renderer *renderer);
        ~RenderWorld_Soft();

        void RenderScene();

    private:
        SoftRenderer *renderer;
        Material *defaultMat;
        Shader *defaultShader;
    };
}

#endif