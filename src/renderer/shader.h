#ifndef __SHADER_H__
#define __SHADER_H__

#include "../shared.h"

namespace CG {

class Shader {
public:
    virtual ~Shader() {}

    virtual const char *ID() const = 0;
protected:
};

}

#endif