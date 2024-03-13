#include "tga.h"

#include <fstream>

using namespace std;

namespace CG {

#define TGA_HEADER_SIZE 18

typedef struct tgaHeader_s {
    int width;
    int height;
    int channels;
    bool rle;
    bool flipH;
    bool flipV;
}tgaHeader_t;

bool ReadHeader(fstream &file, tgaHeader_t& header) {
    unsigned char headerBytes[TGA_HEADER_SIZE];

    file.read((char*)headerBytes, TGA_HEADER_SIZE);

    header.width = headerBytes[12] | (headerBytes[13] << 8);
    header.height = headerBytes[14] | (headerBytes[15] << 8);

    assert(header.width > 0 && header.height > 0);

    assert(headerBytes[16] == 8 || headerBytes[16] == 24 || headerBytes[16] == 32);
    header.channels = headerBytes[16] / 8;

    assert(headerBytes[0] == 0);

    assert(headerBytes[2] == 2 || headerBytes[2] == 3 || headerBytes[2] == 10 || headerBytes[2] == 11);
    header.rle = headerBytes[2] == 10 || headerBytes[2] == 11;

    header.flipH = headerBytes[17] & 0x10;
    header.flipV = headerBytes[17] & 0x20;
   
    return true;
}

void ReadTGARlePayload(fstream &file, image_t &image) {
    int currSize = 0;

    while (currSize < image.len) {
        byte header = Image::ReadByte(file);

        bool rlePacket = header & 0x80;
        int numPixels = (header & 0x7F) + 1;
        byte pixel[4];
        assert(currSize + numPixels * image.channels <= image.len);
        if (rlePacket) {
            for (int i = 0; i < image.channels; i++) {
                pixel[i] = Image::ReadByte(file);
            }

            for (int i = 0; i < numPixels; i++) {
                for (int j = 0; j < image.channels; j++) {
                    image.ldrData[currSize++] = pixel[j];
                }
            }
        }
        else {
            for (int i = 0; i < numPixels; i++) {
                for (int j = 0; j < image.channels; j++) {
                    image.ldrData[currSize++] = Image::ReadByte(file);
                }
            }
        }
    }

    assert(currSize == image.len);
}

int TGA::LoadImage(const char *filename, image_t &image) {
    fstream file(filename, std::ios::in | std::ios::binary);

    if (!file.is_open()) {
        return IMAGE_FILE_OPEN_FAILED;
    }

    tgaHeader_t header;
    ReadHeader(file, header);

    image.channels = header.channels;
    image.format = IMAGE_FORMAT_LDR;
    image.width = header.width;
    image.height = header.height;
    image.len = header.width * header.height * header.channels;
    image.ldrData = new byte[image.len];
    image.hdrData = NULL;

    if (header.rle) {
        ReadTGARlePayload(file, image);
    }
    else {
        Image::ReadBytes(file, image.ldrData, image.len * sizeof(byte));
    }

    file.close();

    if (header.flipH) {
        Image::FlipH<byte>(image, image.ldrData);
    }

    if (header.flipV) {
        Image::FlipV<byte>(image, image.ldrData);
    }

    return IMAGE_OK;
}

}