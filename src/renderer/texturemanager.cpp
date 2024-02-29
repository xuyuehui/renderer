#include "texturemanager.h"
#include "image/bmp.h"
#include "texture.h"

namespace CG {
    TextureManager::TextureManager() {
    }

    void TextureManager::Init() {
    }

    void TextureManager::Shutdown() {
    }

    Texture * TextureManager::LoadTexture(const char *filename) {
        std::map<std::string, Texture *>::iterator it = textures.find(filename);
        if (it != textures.end()) {
            return it->second;
        }

        bitmap_t bitmap;

        if (Bitmap::Read(filename, bitmap) != BMP_OK) {
            return NULL;
        }

        Texture *texture = new Texture(bitmap.width, bitmap.height, bitmap.len, bitmap.data, filename);
        textures.insert(std::pair<std::string, Texture *>(filename, texture));

        return texture;
    }
}