#ifndef __BMP_H__
#define __BMP_H__

namespace CG {
    typedef struct bmp_s {
    }bmp_t;

    bool LoadBMP(const char *filename, bmp_t &bmp);
}

#endif