#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "../shared.h"
#include "../math/vector.h"
#include <string>

namespace CG {

enum TEX_FILTER_TYPE{
    TEXFT_DEFAULT,
    TEXFT_LINEAR,
};

typedef struct image_s image_t;

class Texture {
public:
	Texture(const image_t& image, const char *tag);
    ~Texture();

	void GetSize(int &w, int &h) const;
	const float *Data() const;
        
    Vec4 ColorAt(int x, int y) const;
        
    void SetFilterType(TEX_FILTER_TYPE type);
    TEX_FILTER_TYPE GetFilterType() const;
protected:
	std::string from;
	int width;
	int height;
	int len;
	float *data;
        
    TEX_FILTER_TYPE filterType;
};

inline void Texture::GetSize(int &w, int &h) const {
	w = width;
	h = height;
}

inline const float *Texture::Data() const {
	return data;
}

inline void Texture::SetFilterType(TEX_FILTER_TYPE type) {
    filterType = type;
}

inline TEX_FILTER_TYPE Texture::GetFilterType() const {
    return filterType;
}

}

#endif
