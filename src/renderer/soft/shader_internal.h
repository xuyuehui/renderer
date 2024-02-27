#ifndef __SHADER_INTERNAL_H__
#define __SHADER_INTERNAL_H__

#include "shader_soft.h"

namespace CG {
    class UnlitShader : public Shader_Soft {
    public:
        UnlitShader(string id);

        v2f_t Vertex(const vdata_t &in) const;
        Vec4 Fragment(const v2f_t &in) const;
    };

    inline UnlitShader::UnlitShader(string id) {
        this->id = id;
    }
}

#endif