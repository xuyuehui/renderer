#include "str.h"

namespace CG {

bool Str::EqualTo(const char *str1, const char *str2) {
    assert(str1 && str2);
    return strncmp(str1, str2, strlen(str2)) == 0;
}

bool Str::EndWith(const char *str, const char *pattern) {
    assert(str && pattern);

    size_t plen = strlen(pattern);
    size_t slen = strlen(str);

    if (slen < plen) {
        return false;
    }

    for (int i = 0; i < plen; i++) {
        if (str[slen - i] != pattern[plen - i]) {
            return false;
        }
    }

    return true;
}

const char *Str::FileExtension(const char *path) {
    assert(path);

    size_t len = strlen(path);
    for (int i = len; i >= 0; i--) {
        if (path[i] == '.') {
            return &path[i + 1];
        }
    }

    return path;
}

bool Str::ExtensionEqualTo(const char *path, const char *pattern) {
    return EqualTo(FileExtension(path), pattern);
}

void Str::Combin(char *dst, int len, const char *str1, const char *str2) {
    if (!dst) {
        return;
    }

    if (!str1 || !str2) {
        dst[0] = '\0';
        return;
    }

    int len1 = strlen(str1);
    int len2 = strlen(str2);

    if (len1 + len2 < len) {
        sprintf(dst, "%s%s", str1, str2);
        return;
    }

    len1 = min(len, len1);
    if (len1 > 0) {
        strncpy(dst, str1, len1);
    }

    len2 = min(len2, len);
    if (len2 > 0 && len - len1 > 0) {
        strncpy(&dst[len1], str2, len2);
    }

    dst[len - 1] = '\0';
}

}