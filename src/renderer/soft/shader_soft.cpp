//
//  shader_soft.cpp
//  renderer
//
//  Created by xuyuehui on 2024/3/4.
//

#include "shader_soft.h"
#include "../texture.h"
#include "../model.h"
#include "../material.h"
#include "../renderer.h"

namespace CG {

Vec4 Shader_Soft::Sample2D(const Texture *texture, const Vec2 &texcoord) {
    if (texture == NULL) {
        return Vec4(1.0f, .0f, 1.0f, 1);
    }
        
    if (texture->GetFilterType() == TEXFT_LINEAR) {
        int w = 0, h = 0;
        texture->GetSize(w, h);
            
        float x = clamp(texcoord.x, .0f, 1.0f) * (w-1);
        float y = clamp(texcoord.y, .0f, 1.0f) * (h-1);
            
        float stepX = x - floor(x);
        float stepY = y - floor(y);
            
        Vec4 color0 = texture->ColorAt(x, y);
        Vec4 color1 = texture->ColorAt(x + 1, y);
        Vec4 color2 = texture->ColorAt(x, y + 1);
        Vec4 color3 = texture->ColorAt(x + 1, y + 1);
            
        return Vec4::Lerp(Vec4::Lerp(color0, color1, stepX), 
                            Vec4::Lerp(color2, color3, stepX),
                            stepY);
    } else {
        int w = 0, h = 0;
        texture->GetSize(w, h);
            
        int x = floor(clamp(texcoord.x, .0f, 1.0f) * (w-1));
        int y = floor(clamp(texcoord.y, .0f, 1.0f) * (h-1));
            
        return texture->ColorAt(x, y);
    }
}

}
