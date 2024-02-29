#include "bmp.h"
#include <fstream>

#define BMP_MAGIC 0x4D42
#define BMP_PADDING(a) ((a) % 4)

#define BMP_BYTE_PER_PIXEL 3

namespace CG {
    typedef struct bitmapHeader_s {
        uint32 size;
        uint32 reserved;
        uint32 offset;
        uint32 biSize;
        int32 width;
        int32 height;
        uint16 planes;
        uint16 bitCount;
        uint32 compression;
        uint32 sizeImage;
        int xPelsPerMeter;
        int yPelsPerMeter;
        uint32 clrUsed;
        uint32 clrImportant;
    }bitmapHeader_t;

    int Bitmap::Read(const char *filename, bitmap_t &bitmap) {
        std::ifstream in(filename, std::ios::binary);

        if (!in.is_open()) {
            return BMP_FILE_OPEN_FAILED;
        }

        unsigned short magic = 0;

        in.read(reinterpret_cast<char *>(&magic), sizeof(magic));
        if (magic != BMP_MAGIC) {
            return BMP_INVALID_FILE;
        }

        bitmapHeader_t header;

        in.read(reinterpret_cast<char *>(&header), sizeof(header));

        int h = abs(header.height);
        int offset = (header.height > 0 ? 0 : h - 1);
        int padding = BMP_PADDING(header.width);

        bitmap.len = header.width * h * BMP_BYTE_PER_PIXEL;
        bitmap.data = new byte[bitmap.len];
        bitmap.width = header.width;
        bitmap.height = h;
        bitmap.bytePerPixel = BMP_BYTE_PER_PIXEL;

        for (int y = h - 1; y >= 0; y--) {
            int index = abs(y - offset) * header.width * BMP_BYTE_PER_PIXEL;
            in.read(reinterpret_cast<char *>(&bitmap.data[index]), header.width * BMP_BYTE_PER_PIXEL);
            in.seekg(padding, std::ios::cur);
        }

        in.close();

        return BMP_OK;
    }
}