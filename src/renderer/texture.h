#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "../shared.h"
#include <string>

namespace CG {
	class Texture {
	public:
		Texture(int w, int h, int len, byte *data, const char *tag);

		void GetSize(int &w, int &h) const;
		const byte *Data() const;
	protected:
		std::string from;
		int width;
		int height;
		int len;
		byte *data;
	};

	inline void Texture::GetSize(int &w, int &h) const {
		w = width;
		h = height;
	}

	inline const byte *Texture::Data() const {
		return data;
	}
}

#endif
