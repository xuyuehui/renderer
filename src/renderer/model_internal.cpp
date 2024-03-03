#include "model_internal.h"
#include "../shared.h"
#include "material.h"
#include "texturemanager.h"
#include "shadermanager.h"
#include "../platform/platform.h"
#include "../renderer/renderer.h"

#include <fstream>
#include <vector>

using namespace std;

namespace CG {
    const int MAX_BUFFER_SIZE = 256;

    inline static bool IsEqual(const char* a, const char *b) {
        return strncmp(a, b, strlen(b)) == 0;
    }

    RenderModelInternal::RenderModelInternal() : scale(Vec3(1.0f,  1.0f, 1.0f)) {
    }

    bool RenderModelInternal::InitFromFile(const char *filename){
        ParseDescFile(filename);
        GenerateMesh();
        GenerateMaterial();

        return true;
    }


    int RenderModelInternal::NumSurfaces() const {
        return 1;
    }

    const modelSurface_t *RenderModelInternal::Surface(int surfaceNum) const {
        return &surface;
    }

    void RenderModelInternal::ParseDescFile(const char *filename) {
        ifstream fs(filename, std::ios::in);
        char buffer[MAX_BUFFER_SIZE];
        char tmp[MAX_BUFFER_SIZE];

        if (!fs.is_open()) {
            return;
        }

        while (fs.getline(buffer, MAX_BUFFER_SIZE)) {
            if (strncmp(buffer, "mesh", 4) == 0) {
                if (sscanf(buffer, "mesh %s", tmp) == 1) {
                    tmp[MAX_BUFFER_SIZE - 1] = '\0';
                    meshFilename = tmp;
                }
            }
            else if (strncmp(buffer, "albedo", 6) == 0) {
                if (sscanf(buffer, "albedo %s", tmp) == 1) {
                    tmp[MAX_BUFFER_SIZE - 1] = '\0';
                    albedoMap = tmp;
                }
            }
            else if (strncmp(buffer, "diffuse", 7) == 0) {
                if (sscanf(buffer, "diffuse %s", tmp) == 1) {
                    tmp[MAX_BUFFER_SIZE - 1] = '\0';
                    diffuseMap = tmp;
                }
            }
            else if (strncmp(buffer, "specular", 8) == 0) {
                if (sscanf(buffer, "specular %s", tmp) == 1) {
                    tmp[MAX_BUFFER_SIZE - 1] = '\0';
                    specularMap = tmp;
                }
            }
            else if (strncmp(buffer, "normal", 6) == 0) {
                if (sscanf(buffer, "specular %s", tmp) == 1) {
                    tmp[MAX_BUFFER_SIZE - 1] = '\0';
                    normalMap = tmp;
                }
            }
            else if (strncmp(buffer, "scale", 5) == 0) {
                if (sscanf(buffer, "scale %f %f %f", &scale.x, &scale.y, &scale.z) < 3) {
                    scale = Vec3(1.0f, 1.0f, 1.0f);
                }
            }
        }

        fs.close();
    }

    bool RenderModelInternal::GenerateMesh() {
        if (meshFilename.empty()) {
            return false;
        }

        ifstream fs(meshFilename.c_str(), std::ios::in);

        if (!fs.is_open()) {
            return false;
        }

        char buffer[MAX_BUFFER_SIZE];
        float x, y, z;
        
        std::vector<Vec3> vertices;
        std::vector<Vec2> texcoords;
        std::vector<Vec3> normals;
        std::vector<Vec3i> faces;
        std::vector<Vec3i> faceNormals;
        std::vector<Vec3i> faceTexcoords;
        
        size_t v1, v2, v3, vt1, vt2, vt3, vn1, vn2, vn3;

        while (fs.getline(buffer, MAX_BUFFER_SIZE)) {
            if (IsEqual(buffer, "v ")) {
                if (sscanf(buffer, "v %f %f %f", &x, &y, &z) == 3) {
                    vertices.push_back(Vec3(x, y, z));
                }
            } else if (IsEqual(buffer, "vt ")) {
                if (sscanf(buffer, "vt %f %f", &x, &y) == 2) {
                    texcoords.push_back(Vec2(x, y));
                }
            } else if (IsEqual(buffer, "vn ")) {
                if (sscanf(buffer, "vn %f %f %f", &x, &y, &z) == 3) {
                    normals.push_back(Vec3(x, y, z));
                }
            } else if (IsEqual(buffer, "f ")) {
                if (sscanf(buffer, "f %lu/%lu/%lu %lu/%lu/%lu %lu/%lu/%lu", &v1, &vt1, &vn1, &v2, &vt2, &vn2, &v3, &vt3, &vn3) == 9) {
                    faces.push_back(Vec3i((int)(v1-1), (int)(v2-1), (int)(v3-1)));
                    faceTexcoords.push_back(Vec3i((int)(vt1-1), (int)(vt2-1), (int)(vt3-1)));
                    faceNormals.push_back(Vec3i((int)(vn1-1), (int)(vn2-1), (int)(vn3-1)));
                } else if (sscanf(buffer, "f %lu %lu %lu", &v1, &v2, &v3) == 3) {
                    faces.push_back(Vec3i((int)(v1-1), (int)(v2-1), (int)(v3-1)));
                } else if (sscanf(buffer, "f %lu//%lu %lu//%lu %lu//%lu", &v1, &vn1, &v2, &vn2, &v3, &vn3) == 6) {
                    faces.push_back(Vec3i((int)(v1-1), (int)(v2-1), (int)(v3-1)));
                    faceNormals.push_back(Vec3i((int)(vn1-1), (int)(vn2-1), (int)(vn3-1)));
                } else if (sscanf(buffer, "f %lu/%lu %lu/%lu %lu/%lu", &v1, &vt1, &v2, &vt2, &v3, &vt3) == 6) {
                    faces.push_back(Vec3i((int)(v1-1), (int)(v2-1), (int)(v3-1)));
                    faceTexcoords.push_back(Vec3i((int)(vt1-1), (int)(vt2-1), (int)(vt3-1)));
                }
            }
        }

        fs.close();

        surface.geometry = new srfTriangles_t();

        if (!vertices.empty()) {
            surface.geometry->numVerts = vertices.size();
            surface.geometry->verts = new vertex_t[vertices.size()];

            for (int i = 0; i < vertices.size(); i++) {
                center += vertices[i];
                surface.geometry->verts[i].xyz = vertices[i];
            }

            center *= (1.0f / vertices.size());
        }

        if (!faces.empty()) {
            surface.geometry->numIndexes = faces.size() * 3;
            surface.geometry->indexes = new int[faces.size() * 3];

            for (int i = 0; i < faces.size(); i++) {
                surface.geometry->indexes[i * 3 + 0] = faces[i][0];
                surface.geometry->indexes[i * 3 + 1] = faces[i][1];
                surface.geometry->indexes[i * 3 + 2] = faces[i][2];

                if (!texcoords.empty()) {
                    surface.geometry->verts[faces[i][0]].st = texcoords[faceTexcoords[i][0]];
                    surface.geometry->verts[faces[i][1]].st = texcoords[faceTexcoords[i][1]];
                    surface.geometry->verts[faces[i][2]].st = texcoords[faceTexcoords[i][2]];
                }

                if (!normals.empty()) {
                    surface.geometry->verts[faces[i][0]].normal = normals[faceNormals[i][0]];
                    surface.geometry->verts[faces[i][1]].normal = normals[faceNormals[i][1]];
                    surface.geometry->verts[faces[i][2]].normal = normals[faceNormals[i][2]];
                }
            }
        }

        // 没有顶点法线，自行默认计算
        if (normals.empty()) {
            GenerateSurfaceTriangleExt();

            int *vertexRef = new int[vertices.size()];
            memset(vertexRef, 0, vertices.size() * sizeof(int));

            for (int i = 0; i < faces.size(); i++) {
                vertexRef[faces[i][0]]++;
                vertexRef[faces[i][1]]++;
                vertexRef[faces[i][2]]++;

                surface.geometry->verts[faces[i][0]].normal += surface.geometryExt->normals[i];
            }

            for (int i = 0; i < vertices.size(); i++) {
                if (vertexRef[i] > 0) {
                    surface.geometry->verts[faces[i][0]].normal.Normalized();
                }
            }

            delete[] vertexRef;
        }

        return true;
    }

    void RenderModelInternal::GenerateMaterial() {
        surface.material = new Material();
        surface.material->SetRenderFlags(RF_BACK_FACE_CULLING | RF_DEPTH_TEST);

        Renderer *renderer = App()->GetRenderer();

        surface.material->shader = renderer->GetShaderManager()->LoadShader("internal/unlit");

        if (!albedoMap.empty()) {
            surface.material->albedo = renderer->GetTextureManager()->LoadTexture(albedoMap.c_str());
        }

        if (!diffuseMap.empty()) {
            surface.material->albedo = renderer->GetTextureManager()->LoadTexture(diffuseMap.c_str());
        }

        if (!specularMap.empty()) {
            surface.material->albedo = renderer->GetTextureManager()->LoadTexture(specularMap.c_str());
        }

        if (!normalMap.empty()) {
            surface.material->albedo = renderer->GetTextureManager()->LoadTexture(normalMap.c_str());
        }
    }

    void RenderModelInternal::GenerateSurfaceTriangleExt() {
        if (surface.geometryExt != NULL) {
            return;
        }

        int faceNum = surface.geometry->numIndexes / 3;

        surface.geometryExt = new srfTriangleExts_t();
        surface.geometryExt->normals = new Vec3[faceNum];

        for (int i = 0; i < faceNum; i++) {
            Vec3 e0 = surface.geometry->verts[surface.geometry->indexes[i * 3 + 1]].xyz - surface.geometry->verts[surface.geometry->indexes[i * 3 + 0]].xyz;
            Vec3 e1 = surface.geometry->verts[surface.geometry->indexes[i * 3 + 2]].xyz - surface.geometry->verts[surface.geometry->indexes[i * 3 + 1]].xyz;

            // 计算面法线
            surface.geometryExt->normals[i] = e0.Cross(e1).Normalized();

            //Vec2 uv0 = surface.geometry->verts[faces[i][1]].st - surface.geometry->verts[faces[i][0]].st;
            //Vec2 uv1 = surface.geometry->verts[faces[i][2]].st - surface.geometry->verts[faces[i][1]].st;

            //float f = 1.0f / (uv0.x * uv1.y - uv1.x * uv0.y);
        }
    }

}
