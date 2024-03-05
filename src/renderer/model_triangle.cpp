//
//  model_triangle.cpp
//  renderer
//
//  Created by xuyuehui on 2024/3/5.
//

#include "model_triangle.h"
#include "../shared.h"
#include "material.h"
#include "texturemanager.h"
#include "shadermanager.h"
#include "../platform/platform.h"
#include "../renderer/renderer.h"

namespace CG {
    RenderModelTriangle::RenderModelTriangle() {
    }

    bool RenderModelTriangle::InitFromFile(const char *filename) {
        surface.geometry = new srfTriangles_t();
        
        surface.geometry->numVerts = 3;
        surface.geometry->verts = new vertex_t[3] {
            {
                .xyz = Vec3(-1.0f, 0.0f, 1.0f),
                .normal = Vec3(0.0f, 0.0f, -1.0f),
            },
            
            {
                .xyz = Vec3(0.0f, 1.0f, 2.0f),
                .normal = Vec3(0.0f, 0.0f, -1.0f),
            },
            
            {
                .xyz = Vec3(1.0f, 0.0f, 1.0f),
                .normal = Vec3(0.0f, 0.0f, -1.0f),
            }
        };
        
        surface.geometry->numIndexes = 3;
        surface.geometry->indexes = new int[3] {
            0, 1, 2
        };
        
        return true;
    }

    int RenderModelTriangle::NumSurfaces() const {
        return 1;
    }

    const modelSurface_t* RenderModelTriangle::Surface(int surfaceNum) const {
        return &surface;
    }

    void RenderModelTriangle::GenerateSurfaceTriangleExt() {
    }
}
