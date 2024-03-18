#ifndef __SHADER_PBRM_INTERNAL_H__
#define __SHADER_PBRM_INTERNAL_H__

#include "shader_soft.h"

namespace CG {

namespace Pbrm {

class UnlitShader : public ShaderPbrmBase {
public:
    void Vertex(const ishaderVarying_t *in, ishaderVarying_t *out) const;
    Vec4 Fragment(const fragmentArgs_t *in) const;
};


}

}

#endif