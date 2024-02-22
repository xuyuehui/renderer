#include "frame_buffer.h"

namespace CG {
	FrameBuffer::FrameBuffer() : width(0), height(0), size(0) {
		colorBuffer = NULL;
		depthBuffer = NULL;
	}

	FrameBuffer::FrameBuffer(int width, int height) {
		this->width = width;
		this->height = height;
		this->size = width * height;

		this->colorBuffer = new byte[this->size * 3];
		this->depthBuffer = new float[this->size];

		memset(this->depthBuffer, 0, this->size * sizeof(float));
	}

	FrameBuffer::~FrameBuffer() {
		delete[] colorBuffer;
		delete[] depthBuffer;
	}

	void FrameBuffer::ClearColorBuffer(const rgb &color) {
		int idx = 0;
		for (int i = 0; i < size; i++) {
			colorBuffer[idx] = (byte)floor(color[0] * 255);
			colorBuffer[idx + 1] = (byte)floor(color[1] * 255);
			colorBuffer[idx + 2] = (byte)floor(color[2] * 255);

			idx += 3;
		}
	}

	void FrameBuffer::ClearDepthBuffer(float depth) {
		for (int i = 0; i < size; i++) {
			depthBuffer[i] = depth;
		}
	}
}