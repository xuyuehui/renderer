#ifndef __TEXTURE_MANAGER_H__
#define __TEXTURE_MANAGER_H__

namespace CG {
    class Texture;

    class TextureManager {
    public:
        TextureManager();
        ~TextureManager();

        void Init();
        void Shutdown();
    };
};

#endif