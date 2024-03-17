#include "texture.h"
#include "image/image.h"

namespace CG {

Texture::Texture(const image_t&image, const char *tag) {
    this->width = image.width;
    this->height = image.height;
    this->from = tag;
        
    int len = image.width * image.height * 4;
    this->data = new float[len];
    this->len = len;
        
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int texPos = (y * width + x) * 4;
            int imgPos = (y * width + x) * image.channels;
                
            this->data[texPos + 0] = image.ldrData[imgPos + 0] / 255.0f;
            this->data[texPos + 1] = image.ldrData[imgPos + 1] / 255.0f;
            this->data[texPos + 2] = image.ldrData[imgPos + 2] / 255.0f;
            this->data[texPos + 3] = 1.0f;
        }
    }
        
    this->filterType = TEXFT_DEFAULT;
}

Texture::~Texture() {
    delete[] data;
}

Vec4 Texture::ColorAt(int x, int y) const {        
    x = clamp(x, 0, width-1);
    y = clamp(y, 0, height-1);
        
    return Vec4(&data[(y * width + x) * 4]);
}

}
