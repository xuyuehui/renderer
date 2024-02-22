#ifndef __DEMO_FONT_H__
#define __DEMO_FONT_H__

#include "demo.h"

namespace Tutorial {
    class DemoMain : public Demo {
    public:
        static const char *s_name;
    protected:
        void OnUpdate();
    };
}

#endif
