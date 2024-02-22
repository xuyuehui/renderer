#include "soft_renderer.h"
#include "frame_buffer.h"
#include "..\..\platform\platform.h"
#include "rasterizer.h"
#include "font.h"
#include "renderworld_soft.h"

#include "shadermanager.h"

namespace CG {
	SoftRenderer::SoftRenderer() : bufferIndex(0), needUpdated(false), window(NULL), shaderManager(NULL) {
		buffers[0] = NULL;
		buffers[1] = NULL;
	}

	SoftRenderer::~SoftRenderer() {
		delete buffers[0];
		delete buffers[1];
		delete shaderManager;
	}

	void SoftRenderer::Init(Window* window) {
		if (buffers[0] != NULL) {
			delete buffers[0];
		}

		if (buffers[1] != NULL) {
			delete buffers[1];
		}

		this->window = window;

		int width = 0;
		int height = 0;

		window->GetSize(width, height);

		buffers[0] = new FrameBuffer(width, height);
		buffers[1] = new FrameBuffer(width, height);

		shaderManager = new ShaderManager();
		shaderManager->Init();
	}

	void SoftRenderer::ClearColorBuffer(const rgb& color) {
		GetBackFrameBuffer()->ClearColorBuffer(color);
	}

	void SoftRenderer::ClearDepthBuffer(float depth) {
		GetBackFrameBuffer()->ClearDepthBuffer(depth);
	}

	void SoftRenderer::SwapBuffer() {
		bufferIndex ^= 1;
		GetBackFrameBuffer()->ClearDepthBuffer(0);
		needUpdated = true;
	}

	bool SoftRenderer::GetColorBufferDesc(renderTargetDesc_t& rtd) {
		if (!needUpdated) {
			return false;
		}

		FrameBuffer * frameBuffer = GetFrontFrameBuffer();
		rtd.bitCount = 24;
		rtd.width = frameBuffer->GetWidth();
		rtd.height = frameBuffer->GetHeight();
		rtd.data = frameBuffer->GetColorBuffer();

		needUpdated = false;

		return true;
	}

	void SoftRenderer::DrawLine(const Vec2& s, const Vec2& e, const color_t& color, float depth) {
		FrameBuffer* frameBuffer = GetBackFrameBuffer();
		R_DrawLine(*frameBuffer, s, e, color, depth);
	}

	void SoftRenderer::DrawText(const char* text, const Vec2& pos, float size, const color_t& color, float gap) {
		size_t len = strlen(text);
		FrameBuffer * frameBuffer = GetBackFrameBuffer();
		float x = pos.x;
		float y = pos.y;
		const float ratio = 2.0f;

		for (int i = 0; i < len; ++i) {
			if (text[i] >= 'a' && text[i] <= 'z') {
				Font::R_DrawCharacter(*frameBuffer, x, y, (FONT_CHARACTER)(CHAR_A + text[i] - 'a'), size, color, ratio);
			}
			else if (text[i] >= 'A' && text[i] <= 'Z') {
				Font::R_DrawCharacter(*frameBuffer, x, y, (FONT_CHARACTER)(CHAR_A + text[i] - 'A'), size, color, ratio);
			}
			else if (text[i] == '-') {
				Font::R_DrawCharacter(*frameBuffer, x, y, CHAR_DASH, size, color, ratio);
			}
			else if (text[i] >= '0' && text[i] <= '9') {
				Font::R_DrawCharacter(*frameBuffer, x, y, (FONT_CHARACTER)(CHAR_0 + text[i] - '0'), size, color, ratio);
			}

			x += size * (1.0f + gap);
		}
	}

	RenderWorld *SoftRenderer::CreateRenderWorld() {
		return new RenderWorld_Soft(this);
	}
	 
	uint32_t SoftRenderer::LoadShader(const char *name) {
		return -1;
	}

}
