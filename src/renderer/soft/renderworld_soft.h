#ifndef __RENDERWORLD_SOFT_H__
#define __RENDERWORLD_SOFT_H__

#include "../renderworld_base.h"

namespace CG {

class Renderer;
class SoftRenderer;
class Material;
class Shader;

namespace Blinn {
class Program;
}

namespace Pbrm {
class Program;
}

namespace Pbrs {
class Program;
}

class RenderWorld_Soft : public RenderWorld_Base {
public:
    RenderWorld_Soft(Renderer *renderer);
    ~RenderWorld_Soft();

    void RenderScene();
private:
    SoftRenderer *renderer;
};

class RenderWorldBlinn_Soft : public RenderWorld_Soft {
public:
    RenderWorldBlinn_Soft(Renderer *renderer);
    ~RenderWorldBlinn_Soft();

    virtual IProgram *GetProgram() const;
private:
    Blinn::Program *program;
};

class RenderWorldPbrm_Soft : public RenderWorld_Soft {
public:
    RenderWorldPbrm_Soft(Renderer *renderer);
    ~RenderWorldPbrm_Soft();

    virtual IProgram *GetProgram() const;
private:
    Pbrm::Program * program;
};

class RenderWorldPbrs_Soft : public RenderWorld_Soft {
public:
    RenderWorldPbrs_Soft(Renderer *renderer);
    ~RenderWorldPbrs_Soft();

    virtual IProgram *GetProgram() const;
private:
    Pbrs::Program *program;
};

}

#endif