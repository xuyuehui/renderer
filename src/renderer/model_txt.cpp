#include "model_txt.h"
#include "../shared.h"

#include <fstream>

using namespace std;

namespace CG {
	const int MAX_BUFFER_SIZE = 256;

	RenderModelTxt::RenderModelTxt() : scale(Vec3(1.0f,  1.0f, 1.0f)), material(NULL) {
	}

	bool RenderModelTxt::InitFromFile(const char *filename){
		LoadDescFile(filename);

		return true;
	}

	void RenderModelTxt::LoadDescFile(const char *filename) {
		ifstream fs;
		char buffer[MAX_BUFFER_SIZE];
		char tmp[MAX_BUFFER_SIZE];

		fs.open(filename);

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

	int RenderModelTxt::NumSurfaces() const {
		return 0;
	}

	modelSurface_t * RenderModelTxt::Surface(int surfaceNum) const {
		return NULL;
	}
}
