#ifndef __TGA_H__
#define __TGA_H__

#include "image.h"

namespace CG {

class TGA {
public:
    static int LoadImage(const char *filename, image_t &image);
};

}

#endif
