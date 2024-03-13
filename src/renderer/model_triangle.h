//
//  model_triangle.h
//  renderer
//
//  Created by xuyuehui on 2024/3/5.
//

#ifndef MODEL_TRIANGLE_H
#define MODEL_TRIANGLE_H

#include "model.h"
#include <string>
#include "../math/vector.h"
#include "../utility/list.h"

namespace CG {

class RenderModelTriangle : public RenderModel {
public:
    RenderModelTriangle();

    bool InitFromFile(const char *filename);
    int NumSurfaces() const;
    const modelSurface_t *Surface(int surfaceNum) const;
    void GenerateSurfaceTriangleExt();
    void SetMaterial(int surfaceNum, Material *material) {}
private:
    modelSurface_t surface;
};

}

#endif /* model_triangle_h */
