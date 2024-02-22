#ifndef __SHADER_INTERNAL_H__
#define __SHADER_INTERNAL_H__

#include "shader.h"

namespace CG {
    class UnlitShader : public Shader {
    public:
        const char *Name() const;

        v2f_t Vertex(const vdata_t &in) const;
        Vec4 Fragment(const v2f_t &in) const;
    };
}

#endif