#include "shader_internal.h"

namespace CG {
    const char *UnlitShader::Name() const {
        return "Unlit";
    }

    v2f_t UnlitShader::Vertex(const vdata_t &in) const {
        v2f_t v;
        return v;
    }

    Vec4 UnlitShader::Fragment(const v2f_t &in) const {
        Vec4 color;
        return color;
    }
     
}