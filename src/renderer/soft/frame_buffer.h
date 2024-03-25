#ifndef __FRAME_BUFFER_H__
#define __FRAME_BUFFER_H__

#include "../../shared.h"
#include "../../math/vector.h"

namespace CG {

class FrameBuffer {
public:
	FrameBuffer();
	FrameBuffer(int width, int height);
	~FrameBuffer();

	byte *GetColorBuffer() const;
	float *GetDepthBuffer() const;

	int GetWidth() const;
	int GetHeight() const;
	int GetSize() const;

	void ClearColorBuffer(const Vec3 &color);
	void ClearDepthBuffer(float depth);
private:
	int			width;
	int			height;
	int			size;
	byte		*colorBuffer;
	float		*depthBuffer;
};

inline byte *FrameBuffer::GetColorBuffer() const {
	return colorBuffer; 
}

inline float *FrameBuffer::GetDepthBuffer() const {
	return depthBuffer; 
}

inline int FrameBuffer::GetWidth() const {
	return width; 
}

inline int FrameBuffer::GetHeight() const {
	return height; 
}
	
inline int FrameBuffer::GetSize() const {
	return size; 
}

}

#endif