#ifndef __SHADER_INTERNAL_H__
#define __SHADER_INTERNAL_H__

#include "shader_soft.h"

namespace CG {

namespace Blinn {

class UnlitShader : public ShaderBlinnBase {
public:
    void Vertex(const ishaderVarying_t *in, ishaderVarying_t *out) const;
    Vec4 Fragment(const fragmentArgs_t *in) const;
};

}


}

#endif