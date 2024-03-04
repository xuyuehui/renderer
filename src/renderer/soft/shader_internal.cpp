#include "shader_internal.h"

namespace CG {
    v2f_t UnlitShader::Vertex(const vdata_t &in) const {
        v2f_t out;

        out.position = in.mvpMat * Vec4(in.position, 1.0f);
        out.texcoord = in.texcoord;
        out.normal = in.modelInvTransposeMat * in.normal;
        out.color = Vec4(1.0f, 1.0f, 1.0f, 1.0f);

        return out;
    }

    Vec4 UnlitShader::Fragment(const v2f_t &in) const {
        return Sample2D(in.albedoTex, in.texcoord) * in.color;
    }
     
}
