#include "texture.h"
#include "image/bmp.h"

namespace CG {
    Texture::Texture(const bitmap_t& bmp, const char *tag) {
        this->width = bmp.width;
        this->height = bmp.height;
        this->from = tag;
        
        int len = bmp.width * bmp.height * 4;
        this->data = new float[len];
        this->len = len;
        
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int texPos = (y * width + x) * 4;
                int imgPos = (y * width + x) * bmp.bytePerPixel;
                
                this->data[texPos + 0] = bmp.data[imgPos + 0] / 255.0f;
                this->data[texPos + 1] = bmp.data[imgPos + 1] / 255.0f;
                this->data[texPos + 2] = bmp.data[imgPos + 2] / 255.0f;
                this->data[texPos + 3] = 1.0f;
            }
        }
        
        this->filterType = TEXFT_DEFAULT;
    }

    Texture::~Texture() {
        delete[] data;
    }

    Vec4 Texture::ColorAt(int x, int y) const {
        assert(x >= 0 && x < width);
        assert(y >= 0 && y < height);
        
        x = clamp(x, 0, width-1);
        y = clamp(y, 0, height-1);
        
        return Vec4(&data[(y * width + x) * 4]);
    }
}
