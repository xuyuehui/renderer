#ifndef __SOFT_RENDERER__
#define __SOFT_RENDERER__

#include "../renderer.h"
#include "../../utility/utility.h"

namespace CG {
	class FrameBuffer;
	class ShaderManager;
	class Shader;

	class SoftRenderer : public Renderer {
	public:
		ShaderManager *shaderManager;

		SoftRenderer();
		~SoftRenderer();

		void Init(Window *window);
		void ClearColorBuffer(const rgb &color);
		void ClearDepthBuffer(float depth);
		void SwapBuffer();

		bool GetColorBufferDesc(renderTargetDesc_t &rtd);

		void DrawLine(const Vec2 &s, const Vec2 &e, const color_t &color, float depth);
		void DrawText(const char *text, const Vec2 &pos, float size, const color_t &color, float gap);

		virtual uint32 LoadShader(const char *name);

		virtual RenderWorld *CreateRenderWorld();
	public:
		// 已经准备好的缓冲区
		FrameBuffer *GetFrontFrameBuffer() const;

		// 当前帧正在更改的缓冲区
		FrameBuffer *GetBackFrameBuffer() const;
	private:
		FrameBuffer *buffers[2];
		byte bufferIndex;
		bool needUpdated;

		Window *window;
	};

	inline FrameBuffer *SoftRenderer::GetFrontFrameBuffer() const {
		return buffers[bufferIndex];
	}

	inline FrameBuffer *SoftRenderer::GetBackFrameBuffer() const {
		return buffers[bufferIndex ^ 1];
	}
}

#endif