#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "../../shared.h"
#include <fstream>

namespace CG {

typedef enum {
    IMAGE_OK = 0,
    IMAGE_FILE_OPEN_FAILED,
    IMAGE_INVALID_FILE,
}ImageError;

typedef enum {
    IMAGE_FORMAT_LDR,
    IMAGE_FORMAT_HDR,
}ImageFormat;

typedef struct image_s {
    int width;
    int height;
    int len;
    int channels;
    int format;
    byte *ldrData;
    float *hdrData;
}image_t;

class Image {
public:
    template <typename T> static void Swap(T *a, T *b);
    template <typename T> static void FlipH(image_t &image, T *data);
    template <typename T> static void FlipV(image_t &image, T *data);

    static byte ReadByte(std::fstream &file);
    static void ReadBytes(std::fstream &file, byte *buffer, int size);
};

template<typename T>
void Image::Swap(T *a, T *b) {
    T c;
    c = *a;
    *a = *b;
    *b = c;
}

template <typename T>
void Image::FlipH(image_t &image, T *data) {
    int halfWidth = image.width / 2;

    for (int y = 0; y < image.height; y++) {
        for (int x = 0; x < halfWidth; x++) {
            int flipedX = image.width - x - 1;
            T *pixel1 = &data[(y * image.width + x) * image.channels];
            T *pixel2 = &data[(y * image.width + flipedX) * image.channels];
            for (int k = 0; k < image.channels; k++) {
                Swap<T>(pixel1, pixel2);
            }
        }
    }
}

template <typename T>
void Image::FlipV(image_t &image, T *data) {
    int halfHeight = image.height / 2;

    for (int x = 0; x < image.width; x++) {
        for (int y = 0; y < halfHeight; y++) {
            int flipedY = image.height - y - 1;
            T *pixel1 = &data[(y * image.width + x) * image.channels];
            T *pixel2 = &data[(y * image.width + flipedY) * image.channels];
            for (int k = 0; k < image.channels; k++) {
                Swap<T>(pixel1, pixel2);
            }
        }
    }
}

}

#endif
