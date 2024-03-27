#ifndef __SCENE_LOADER_H__
#define __SCENE_LOADER_H__

#include "../shared.h"
#include "../math/vector.h"
#include "../math/matrix.h"

#include <vector>

namespace CG {

class Renderer;
class RenderModel;
class Texture;
typedef struct renderLight_s renderLight_t;
typedef struct renderEntity_s renderEntity_t;

typedef struct renderScene_s {
    ~renderScene_s();

    renderLight_t *light;
    std::vector<renderEntity_t *> entities;
    int shadingMode;
    Vec3 background;

    Vec2i shadowSize;
}renderScene_t;

class SceneLoader {
public:
    static renderScene_t *LoadScene(Renderer *renderer, const char *filename, const Mat4 &root);
};

}

#endif