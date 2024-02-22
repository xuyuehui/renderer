#ifndef __FONT_H__
#define __FONT_H__

namespace CG {
    class FrameBuffer;
    typedef struct color_s color_t;

    enum FONT_CHARACTER
    {
        CHAR_0,
        CHAR_1,
        CHAR_2,
        CHAR_3,
        CHAR_4,
        CHAR_5,
        CHAR_6,
        CHAR_7,
        CHAR_8,
        CHAR_9,
        CHAR_A,
        CHAR_B,
        CHAR_C,
        CHAR_D,
        CHAR_E,
        CHAR_F,
        CHAR_G,
        CHAR_H,
        CHAR_I,
        CHAR_J,
        CHAR_K,
        CHAR_L,
        CHAR_M,
        CHAR_N,
        CHAR_O,
        CHAR_P,
        CHAR_Q,
        CHAR_R,
        CHAR_S,
        CHAR_T,
        CHAR_U,
        CHAR_V,
        CHAR_W,
        CHAR_X,
        CHAR_Y,
        CHAR_Z,
        CHAR_DASH,
        CHAR_NUM,
    };

    class Font {
    public:
        static void R_DrawCharacter(const FrameBuffer &frameBuffer, float x, float y, FONT_CHARACTER character, float size, const color_t &color, float ratio);
    };
}

#endif
