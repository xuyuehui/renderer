#ifndef __BMP_H__
#define __BMP_H__

#include "image.h"

namespace CG {
    class Bitmap {
    public:
        static int LoadImage(const char *filename, image_t& image);
    };
}

#endif