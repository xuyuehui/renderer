#include "texture.h"
#include "image/image.h"

namespace CG {

Texture::Texture(const byte *data, int width, int height, int channels, const char *tag) {
    this->width = width;
    this->height = height;
    this->tag = tag;

    int len = width * height * 4;
    this->data = new float[len];
    this->len = len;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int texPos = (y * width + x) * 4;
            int imgPos = (y * width + x) * channels;

            this->data[texPos + 0] = data[imgPos + 0] / 255.0f;
            this->data[texPos + 1] = data[imgPos + 1] / 255.0f;
            this->data[texPos + 2] = data[imgPos + 2] / 255.0f;
            this->data[texPos + 3] = channels <= 3 ? 1.0f : data[imgPos + 3] / 255.0f;
        }
    }

    this->filterType = TEXFT_DEFAULT;
}

Texture::Texture(int width, int height) {
    this->width = width;
    this->height = height;
    this->tag = "user created";

    int len = width * height * 4;
    this->data = new float[len];
    this->len = len;

    this->filterType = TEXFT_DEFAULT;
}

Texture::~Texture() {
    delete[] data;
}

Vec4 Texture::ColorAt(int x, int y) const {
    x = clamp(x, 0, width - 1);
    y = clamp(y, 0, height - 1);

    return Vec4(&data[(y * width + x) * 4]);
}

void Texture::SetColor(int index, const Vec4 &color) {
    index *= 4;
    assert(index >= 0 && index < len);

    data[index] = color.x;
    data[index+1] = color.y;
    data[index+2] = color.z;
    data[index+3] = color.w;
}


}
