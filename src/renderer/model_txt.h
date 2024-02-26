#ifndef __MODEL_TXT_H__
#define __MODEL_TXT_H__

#include "model.h"
#include <string>
#include "../math/vector.h"

namespace CG {
    class RenderModelTxt : public RenderModel {
    public:
        RenderModelTxt();

        bool InitFromFile(const char *filename);

        int NumSurfaces() const;

        modelSurface_t *Surface(int surfaceNum) const;
    protected:
        void LoadDescFile(const char *filename);
    private:
        std::string meshFilename;
        std::string albedoMap;
        std::string diffuseMap;
        std::string specularMap;
        std::string normalMap;
        Vec3 scale;

        Material *material;
    };
}

#endif