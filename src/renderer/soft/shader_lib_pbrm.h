#ifndef __SHADER_PBRM_INTERNAL_H__
#define __SHADER_PBRM_INTERNAL_H__

#include "shader_soft_pbrm.h"

namespace CG {

namespace Pbrm {

class CommonShader : public ShaderPbrmBase {
public:
    Vec4 Vertex(const ishaderVertexAttribs_t *attribs, const ishaderUniforms_t *uniforms, ishaderVarying_t *varyings) const;
    Vec4 Fragment(const ishaderVarying_t *in, const ishaderUniforms_t *uniforms) const;
};


}

}

#endif