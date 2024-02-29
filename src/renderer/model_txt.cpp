#include "model_txt.h"
#include "../shared.h"
#include "material.h"
#include "texturemanager.h"
#include "shadermanager.h"
#include "../platform/platform.h"
#include "../renderer/renderer.h"

#include <fstream>

using namespace std;

namespace CG {
	const int MAX_BUFFER_SIZE = 256;

	RenderModelTxt::RenderModelTxt() : scale(Vec3(1.0f,  1.0f, 1.0f)) {
	}

	bool RenderModelTxt::InitFromFile(const char *filename){
		ParseDescFile(filename);
		GenerateMesh();
		GenerateMaterial();

		return true;
	}

	void RenderModelTxt::ParseDescFile(const char *filename) {
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

	bool RenderModelTxt::GenerateMesh() {
		if (meshFilename.empty()) {
			return false;
		}

		ifstream fs(meshFilename.c_str(), std::ios::in);

		if (!fs.is_open()) {
			return false;
		}

		char buffer[MAX_BUFFER_SIZE];
		while (fs.getline(buffer, MAX_BUFFER_SIZE)) {
		}

		fs.close();

		return true;
	}

	void RenderModelTxt::GenerateMaterial() {
		surface.material = new Material();

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

	int RenderModelTxt::NumSurfaces() const {
		return 0;
	}

	const modelSurface_t * RenderModelTxt::Surface(int surfaceNum) const {
		return &surface;
	}
}
