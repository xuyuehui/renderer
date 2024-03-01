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

    // ��������չ��Ϣ���淨�ߡ����ߡ�������
    typedef struct srfTriangleExts_s {
        srfTriangleExts_s() : normals(NULL), tangents(NULL), bitangents(NULL) {
        }

        Vec3 *normals;
        Vec3 *tangents;
        Vec3 *bitangents;
    }srfTriangleExts_t;

    typedef struct modelSurface_s {
        modelSurface_s() : id(0), material(NULL), geometry(NULL), geometryExt(NULL) {
        }

        int id;
        Material *material;
        srfTriangles_t *geometry;
        srfTriangleExts_t *geometryExt;
    }modelSurface_t;

    class RenderModel {
    public:
        virtual ~RenderModel() {}

        virtual bool InitFromFile(const char *filename) = 0;
        
        virtual int NumSurfaces() const = 0;
        
        virtual const modelSurface_t *Surface(int nurfaceNum) const = 0;

        virtual void GenerateSurfaceTriangleExt() = 0;
    };
}

#endif