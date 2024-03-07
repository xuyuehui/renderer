#include "shader_internal.h"

namespace CG {
    void UnlitShader::Vertex(const ishaderVarying_t *in, ishaderVarying_t *out) const {
        const shaderVaryingLocal_t *inVarying = static_cast<const shaderVaryingLocal_t *>(in);
        shaderVaryingLocal_t *outVarying = static_cast<shaderVaryingLocal_t *>(out);

        outVarying->position = inVarying->mvp * inVarying->position;
        outVarying->texcoord = inVarying->texcoord;
        outVarying->color = Vec4(1.0f, 1.0f, 1.0f, 1.0f);
    }

    Vec4 UnlitShader::Fragment(const fragmentArgs_t *in) const {
        const shaderVaryingLocal_t *inVarying = static_cast<const shaderVaryingLocal_t *>(in->varying);
        return Sample2D(in->textures[0], inVarying->texcoord) * inVarying->color;
    }
     
}
