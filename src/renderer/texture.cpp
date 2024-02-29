#include "texture.h"

namespace CG {
    Texture::Texture(int w, int h, int len, byte *data, const char *tag) {
        this->width = w;
        this->height = h;
        this->len = len;
        this->data = data;
        this->from = tag;
    }
}
