#ifndef __SHADER_INTERNAL_H__
#define __SHADER_INTERNAL_H__

#include "shader_soft_blinn.h"

namespace CG {

namespace Blinn {

class CommonShader : public ShaderBlinnBase {
public:
    Vec4 Vertex(const ishaderVertexAttribs_t *attribs, const ishaderUniforms_t *uniforms, ishaderVarying_t *varyings) const;
    Vec4 Fragment(const ishaderVarying_t *varyings, const ishaderUniforms_t *uniforms, bool backface, bool &discard) const;
};

}


}

#endif