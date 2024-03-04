#ifndef __DEMO_BANK_H__
#define __DEMO_BANK_H__

#include "demo.h"

namespace Tutorial {
    class DemoBank : public Demo {
    public:
        static const char *s_name;

    protected:
        void OnInit();
        void OnUpdate();
    };
}

#endif
