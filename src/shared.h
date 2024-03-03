#ifndef __SHARED_H__
#define __SHARED_H__

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

namespace CG {
#undef max
#undef min
#define max(a, b) ((a) > (b)?(a) : (b))
#define min(a, b) ((a) < (b)?(a) : (b))
#define clamp(a, b, c) (min(max((a), (b)), (c)))

#define __unused_variable(var) (void(var))

#define PI 3.14159265358979323846264338327950288f
#define EPSILON 1e-6f

    typedef unsigned char			byte;
    typedef unsigned short			uint16;
    typedef short					int16;
    typedef unsigned int			uint32;
    typedef int						int32;

    typedef unsigned short			word;
    typedef unsigned int			dword;
    typedef unsigned long			ulong;

    const uint32 RF_BACK_FACE_CULLING       = 0x00000001;
    const uint32 RF_WIREFRAME_MODE          = 0x00000002;
    const uint32 RF_DEPTH_TEST              = 0x00000004;
    const uint32 RF_TRANSPARENT             = 0x00000008;
}

#endif
