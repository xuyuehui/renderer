#include "rasterizer.h"
#include "frame_buffer.h"

using namespace std;

namespace CG {
	void R_DrawPixel(const FrameBuffer &frameBuffer, int x, int y, const color_t &color, float depth) {
		int depthBufferPos = frameBuffer.GetWidth() * y + x;
		float d = clamp(depth, -1.0f, 1.0f);
		byte *colorBuffer = frameBuffer.GetColorBuffer();
		float *depthBuffer = frameBuffer.GetDepthBuffer();

		if (depthBuffer[depthBufferPos] <= d) {
			return;
		}

		depthBuffer[depthBufferPos] = d;
		int colorBufferPos = (frameBuffer.GetWidth() * y + x) * 3;
		colorBuffer[colorBufferPos] = color.r;
		colorBuffer[colorBufferPos + 1] = color.g;
		colorBuffer[colorBufferPos + 2] = color.b;
	}

	void R_DrawLine(const FrameBuffer &frameBuffer, const Vec2 &v1, const Vec2 &v2, const color_t &color, float depth) {
		int x1 = clamp((int)floor(v1.x), 0, frameBuffer.GetWidth());
		int y1 = clamp((int)floor(v1.y), 0, frameBuffer.GetHeight());
		int x2 = clamp((int)floor(v2.x), 0, frameBuffer.GetWidth());
		int y2 = clamp((int)floor(v2.y), 0, frameBuffer.GetHeight());

		int dx = abs(x2 - x1);
		int dy = -abs(y2 - y1);
		int sx = x1 < x2 ? 1 : -1;
		int sy = y1 < y2 ? 1 : -1;

		int err = dx + dy;

		while (true) {
			R_DrawPixel(frameBuffer, x1, y1, color, depth);

			if (x1 == x2 && y1 == y2) {
				break;
			}

			int err2 = 2 * err;
			if (err2 > dy) {
				err += dy;
				x1 += sx;
			}

			if (err2 < dx) {
				err += dx;
				y1 += sy;
			}
		}
	}
}
