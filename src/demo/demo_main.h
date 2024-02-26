#ifndef __DEMO_FONT_H__
#define __DEMO_FONT_H__

#include "demo.h"

namespace CG {
    class RenderModel;
}

namespace Tutorial {
   
    class DemoMain : public Demo {
    public:
        static const char *s_name;

        DemoMain();
    protected:
        void OnInit();
        void OnUpdate();
        void OnShutdown();

    private:
        CG::RenderModel *model;
    };
}

#endif
