#ifndef __SHADER_PBRS_INTERNAL_H__
#define __SHADER_PBRS_INTERNAL_H__

#include "shader_soft_pbrs.h"

namespace CG {

namespace Pbrs {

class CommonShader : public ShaderPbrsBase {
public:
    Vec4 Vertex(const ishaderVertexAttribs_t *attribs, const ishaderUniforms_t *uniforms, ishaderVarying_t *varyings) const;
    Vec4 Fragment(const ishaderVarying_t *varyings, const ishaderUniforms_t *uniforms) const;
};

}

}

#endif