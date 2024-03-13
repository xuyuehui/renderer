#ifndef __STR_H__
#define __STR_H__

#include "../shared.h"

namespace CG {

class Str {
public:
    static bool EqualTo(const char *str1, const char *str2);
    static bool EndWith(const char *str, const char *pattern);
    static const char *FileExtension(const char *path);
    static bool ExtensionEqualTo(const char *path, const char *pattern);
    static void Combin(char *dst, int len, const char *str1, const char *str2);
};

}

#endif