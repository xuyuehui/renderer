#include "texturemanager.h"
#include "image/bmp.h"
#include "image/tga.h"
#include "texture.h"
#include "../utility/str.h"

namespace CG {

TextureManager::TextureManager() {
}

void TextureManager::Init() {
}

void TextureManager::Shutdown() {
    for (std::map<std::string, Texture *>::iterator it = textures.begin(); it != textures.end(); it++) {
        delete it->second;
    }

    textures.clear();
}

Texture * TextureManager::LoadTexture(const char *filename) {
    if (!filename) {
        return NULL;
    }

    std::map<std::string, Texture *>::iterator it = textures.find(filename);
    if (it != textures.end()) {
        return it->second;
    }
        
    image_t image;
    int code;
    const char *ext = Str::FileExtension(filename);

    if (Str::EqualTo(ext, "bmp")) {
        code = Bitmap::LoadImage(filename, image);
    }
    else if (Str::EqualTo(ext, "tga")) {
        code = TGA::LoadImage(filename, image);
    }
    else {
        assert(false);
        return NULL;
    }

    if (code != IMAGE_OK) {
        return NULL;
    }

    Texture *texture = new Texture(image.ldrData, image.width, image.height, image.channels, filename);

    textures.insert(std::pair<std::string, Texture *>(filename, texture));

    delete[] image.ldrData;
    delete[] image.hdrData;

    return texture;
}

}
