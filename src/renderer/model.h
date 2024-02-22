#ifndef __MODEL_H__
#define __MODEL_H__

#include "../shared.h"
#include "../math/vector.h"

namespace CG {
    class Material;

    typedef struct vertex_s {
        Vec3 xyz;
        Vec2 st;
        Vec3 normal;
        Vec3 tangent;
        Vec4 color;
    }vertex_t;

    typedef struct srfTriangles_s {
        int numVerts;
        vertex_t *verts;

        int numIndexes;
        int *indexes;
    }srfTriangles_t;

    typedef struct modelSurface_s {
        int id;
        const Material *material;
        srfTriangles_t *geometry;
    }modelSurface_t;

    class RenderModel {
    public:
        virtual ~RenderModel() {}

        virtual bool InitFromFile(const char *filename) = 0;
        
        virtual int NumSurfaces() const = 0;
        
        virtual modelSurface_t *Surface(int nurfaceNum) const = 0;
    };
}

#endif