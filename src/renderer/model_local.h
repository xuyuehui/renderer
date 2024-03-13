#ifndef __MODEL_LOCAL_H__
#define __MODEL_LOCAL_H__

#include "model.h"
#include <string>
#include "../math/vector.h"
#include "../utility/list.h"

namespace CG {

class RenderModelObj : public RenderModel {
public:
    RenderModelObj();

    bool InitFromFile(const char *filename);
    int NumSurfaces() const;
    const modelSurface_t *Surface(int surfaceNum) const;
    void GenerateSurfaceTriangleExt();
    void SetMaterial(int surfaceNum, Material *material);
protected:
    modelSurface_t surface;
    Vec3 center;
    Vec3 scale;
};

class RenderModelSimple : public RenderModelObj {
public:
    struct decl_t {
        std::string meshFilename;
        std::string emissionMap;
        std::string diffuseMap;
        std::string specularMap;
        std::string normalMap;
    };

    bool InitFromFile(const char *filename);
    void GenerateSurfaceTriangleExt();
protected:
    decl_t ReadDecl(const char *filename);
    void ReadMaterial(const decl_t& desc);
};

}

#endif
