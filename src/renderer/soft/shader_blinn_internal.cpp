#include "shader_blinn_internal.h"

namespace CG {

namespace Blinn {

void UnlitShader::Vertex(const ishaderVarying_t *in, ishaderVarying_t *out) const {
    const shaderVaryingLocal_t *inVarying = static_cast<const shaderVaryingLocal_t *>(in);
    shaderVaryingLocal_t *outVarying = static_cast<shaderVaryingLocal_t *>(out);

    Vec4 pos = inVarying->model * inVarying->position;

    outVarying->position = inVarying->mvp * inVarying->position;
    outVarying->texcoord = inVarying->texcoord;
    outVarying->color = Vec4(1.0f, 1.0f, 1.0f, 1.0f);
}

Vec4 UnlitShader::Fragment(const fragmentArgs_t *in) const {
    const shaderVaryingLocal_t *inVarying = static_cast<const shaderVaryingLocal_t *>(in->varying);
    return Sample2D(TexDiffuse(in), inVarying->texcoord) * inVarying->color;
}

}
     
}
