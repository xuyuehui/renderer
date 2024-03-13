#include "image.h"

namespace CG {

byte Image::ReadByte(std::fstream &file) {
    byte data;
    file.read((char *)&data, 1);
    return data;
}

void Image::ReadBytes(std::fstream &file, byte * buffer, int size) {
    file.read((char *)buffer, size);
}

}