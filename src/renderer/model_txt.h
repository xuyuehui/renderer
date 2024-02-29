#ifndef __MODEL_TXT_H__
#define __MODEL_TXT_H__

#include "model.h"
#include <string>
#include "../math/vector.h"
#include "../utility/list.h"

namespace CG {
    class RenderModelTxt : public RenderModel {
    public:
        RenderModelTxt();

        bool InitFromFile(const char *filename);

        int NumSurfaces() const;

        const modelSurface_t *Surface(int surfaceNum) const;
    protected:
        void ParseDescFile(const char *filename);
        bool GenerateMesh();
        void GenerateMaterial();
    private:
        std::string meshFilename;
        std::string albedoMap;
        std::string diffuseMap;
        std::string specularMap;
        std::string normalMap;
        Vec3 scale;

        modelSurface_t surface;
    };
}

#endif