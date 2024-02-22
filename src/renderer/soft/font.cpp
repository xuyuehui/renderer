#include <assert.h>
#include "font.h"
#include "rasterizer.h"
#include "..\..\math\vector.h"

namespace CG {
    // 16-segment character data
    const int32_t CHARS_16SEG[] = {
        0b1110011001100111, // 0
        0b0000001000000100, // 1
        0b1100001111000011, // 2
        0b1100001110000111, // 3
        0b0010001110000100, // 4
        0b1110000110000111, // 5
        0b1110000111000111, // 6
        0b1100001000000100, // 7
        0b1110001111000111, // 8
        0b1110001110000111, // 9
        0b1110001111000100, // A
        0b1110001111000111, // B
        0b1110000001000011, // C
        0b1110001001000111, // D
        0b1110000111000011, // E
        0b1110000111000000, // F
        0b1110000011000111, // G
        0b0010001111000100, // H
        0b1100100000010011, // I
        0b1100100000010010, // J
        0b0010010101001000, // K
        0b0010000001000011, // L
        0b0011011001000100, // M
        0b0011001001001100, // N
        0b1110001001000111, // O
        0b1110001111000000, // P
        0b1110001001001111, // Q
        0b1110001111001000, // R
        0b1110000110000111, // S
        0b1100100000010000, // T
        0b0010001001000111, // U
        0b0010001001000111, // V
        0b0010001001101100, // W
        0b0001010000101000, // X
        0b0001010000010000, // Y
        0b1100010000100011, // Z
        0b0000000110000000, // - (DASH)
    };

    /**
     * 16-segment digit vertices coords
     *   u ---▶
     * v 0 -- 1 -- 2
     * | | \  |  / |
     * ▼ |  \ | /  |
     *   3 -- 4 -- 5
     *   |  / | \  |
     *   | /  |  \ |
     *   6 -- 7 -- 8
     */
    const float CHARS_16SEG_COORDS[][2] = {
        { 0.0f, 0.0f },
        { 0.5f, 0.0f },
        { 1.0f, 0.0f },
        { 0.0f, 0.5f },
        { 0.5f, 0.5f },
        { 1.0f, 0.5f },
        { 0.0f, 1.0f },
        { 0.5f, 1.0f },
        { 1.0f, 1.0f },
    };

    const int CHARS_16SEG_EDGES[][2] = {
        { 0, 1 }, { 1, 2 }, { 0, 3 }, { 0, 4 },
        { 1, 4 }, { 2, 4 }, { 2, 5 }, { 3, 4 },
        { 4, 5 }, { 3, 6 }, { 4, 6 }, { 4, 7 },
        { 4, 8 }, { 5, 8 }, { 6, 7 }, { 7, 8 },
    };

    void Font::R_DrawCharacter(const FrameBuffer &frameBuffer, float x, float y, FONT_CHARACTER character, float size, const color_t &color, float ratio) {
        assert(character >= 0 && character < CHAR_NUM);

        for (int i = 0; i < 16; i++) {
            if ((CHARS_16SEG[(int)character] >> (15 - i) & 0x01) != 0) {
                R_DrawLine(frameBuffer,
                    Vec2(x + CHARS_16SEG_COORDS[CHARS_16SEG_EDGES[i][0]][0] * size, y + CHARS_16SEG_COORDS[CHARS_16SEG_EDGES[i][0]][1] * size * ratio),
                    Vec2(x + CHARS_16SEG_COORDS[CHARS_16SEG_EDGES[i][1]][0] * size, y + CHARS_16SEG_COORDS[CHARS_16SEG_EDGES[i][1]][1] * size * ratio),
                    color, -1.0f);
            }
        }
    }
}