#ifndef __TEXTURE_MANAGER_H__
#define __TEXTURE_MANAGER_H__

#include <map>
#include <string>

namespace CG {
    class Texture;

    class TextureManager {
    public:
        TextureManager();

        void Init();
        void Shutdown();

        Texture *LoadTexture(const char *filename);
    private:
        std::map<std::string, Texture *> textures;
    };
};

#endif