#ifndef __BMP_H__
#define __BMP_H__

#include "../../shared.h"

namespace CG {
    typedef struct bitmap_s {
        int width;
        int height;
        int len;
        int bytePerPixel;
        byte *data;
    }bitmap_t;

    typedef enum {
        BMP_OK = 0,
        BMP_FILE_OPEN_FAILED,
        BMP_INVALID_FILE,
    }BmpError;

    class Bitmap {
    public:
        static int Read(const char *filename, bitmap_t &bitmap);
    };
}

#endif