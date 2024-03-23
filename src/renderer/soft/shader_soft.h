#ifndef __SHADER_SOFT_H__
#define __SHADER_SOFT_H__

#include "../shader.h"
#include "../../math/vector.h"
#include "../../math/matrix.h"
#include <string>

using namespace std;

namespace CG {

class Texture;
class Material;
typedef struct vertex_s vertex_t;

static const int MAX_VARYINGS = 10;
static const int MAX_TEXTURES = 8;

typedef struct ishaderVarying_s {
}ishaderVarying_t;

typedef struct ishaderVertexAttribs_s {
    Vec4 position;
} ishaderVertexAttribs_t;

typedef struct ishaderUniforms_s {
    Mat4 modelMat;
    Mat4 viewMat;
    Mat4 projMat;
    Mat4 vpCameraMat;
    Mat4 vpLightMat;
}ishaderUniforms_t;

class IProgram {
public:
    Shader *shader;
    ishaderVarying_t *shaderVarying;

    ishaderVertexAttribs_t *attribs[3];

    Vec4 inCoords[MAX_VARYINGS];
    Vec4 outCoords[MAX_VARYINGS];

    ishaderVarying_t *inVaryings[MAX_VARYINGS];
    ishaderVarying_t *outVaryings[MAX_VARYINGS];

    ishaderUniforms_t *uniforms;

    bool doubleSided;
public:
    virtual ~IProgram() {}

    virtual void SetupMaterial(const Material *material) = 0;
    virtual void SetupVertex(const vertex_t &v, ishaderVertexAttribs_t *attrib, ishaderVarying_t *varying) = 0;
    virtual void Interpolate(ishaderVarying_t *in[3], const Vec3 &weights, float recipW[3], ishaderVarying_t *out) = 0;
    virtual void Interpolate(ishaderVarying_t *src, ishaderVarying_t *dst, float ratio, ishaderVarying_t *out) = 0;
    virtual void CopyFrom(const ishaderVarying_t *src, ishaderVarying_t *dst) = 0;
};

class ProgramBase : public IProgram {
protected:
    /*
     * for perspective correct interpolation, see
     * https://www.comp.nus.edu.sg/~lowkl/publications/lowk_persp_interp_techrep.pdf
     * https://www.khronos.org/registry/OpenGL/specs/es/2.0/es_full_spec_2.0.pdf
     *
     * equation 15 in reference 1 (page 2) is a simplified 2d version of
     * equation 3.5 in reference 2 (page 58) which uses barycentric coordinates
     */
    template <typename T>
    void InterpolateT(ishaderVarying_t *in[3], const Vec3 &weights, float recipW[3], ishaderVarying_t *out) {
        int numOfFloats = sizeof(T) / sizeof(float);
        float *dst = reinterpret_cast<float *>(out);
        float *src0 = reinterpret_cast<float *>(in[0]);
        float *src1 = reinterpret_cast<float *>(in[1]);
        float *src2 = reinterpret_cast<float *>(in[2]);

        float weight0 = recipW[0] * weights.x;
        float weight1 = recipW[1] * weights.y;
        float weight2 = recipW[2] * weights.z;
        float normalizer = 1.0f / (weight0 + weight1 + weight2);

        for (int i = 0; i < numOfFloats; i++) {
            dst[i] = src0[i] * weight0 + src1[i] * weight1 + src2[i] * weight2;
            dst[i] *= normalizer;
        }
    }

    template <typename T>
    void InterpolateT(ishaderVarying_t *src, ishaderVarying_t *dst, float ratio, ishaderVarying_t *out) {
        int numOfFloats = sizeof(T) / sizeof(float);
        float *outf = reinterpret_cast<float *>(out);
        float *srcf = reinterpret_cast<float *>(src);
        float *dstf = reinterpret_cast<float *>(dst);

        for (int i = 0; i < numOfFloats; i++) {
            outf[i] = srcf[i] * ratio + dstf[i] * (1.0f - ratio);
        }
    }

    template <typename T>
    void CopyFromT(const ishaderVarying_t *src, ishaderVarying_t *dst) {
        memcpy((void *)dst, src, sizeof(T));
    }
};

class Shader_Soft : public Shader {
public:
    virtual ~Shader_Soft() {}

    virtual Vec4 Vertex(const ishaderVertexAttribs_t *attribs, const ishaderUniforms_t *uniforms, ishaderVarying_t *varyings) const = 0;
    virtual Vec4 Fragment(const ishaderVarying_t *in, const ishaderUniforms_t *uniforms) const = 0;

    static Vec4 Sample2D(const Texture *texture, const Vec2 &texcoord);
};

}

#endif
