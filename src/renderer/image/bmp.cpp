#include "bmp.h"
#include "../../shared.h"

#define LBI_BM 0x4d42    // ASCII code for 'BM'
#define LBI_FILE_HEADER_SIZE 14L

#define LBI_RGB  0   // no compression
#define LBI_RLE8 1   // 8bit RLE encoding
#define LBI_RLE4 2   // 4bit RLE encoding

namespace CG {
    typedef struct bmpFileHeader_s {
        uint16 signature;
        uint32 fileSize;
        uint32 reserved;
        uint32 offset;
    }bmpFileHeader_t;

    bool LoadBMP(const char *filename, bmp_t &bmp) {
        FILE *fp = fopen(filename, "rb");
        if (!fp) {
            return false;
        }

        return true;
    }
}