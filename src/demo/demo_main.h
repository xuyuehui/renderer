#ifndef __DEMO_FONT_H__
#define __DEMO_FONT_H__

#include "demo.h"

namespace CG {
    typedef struct renderEntity_s renderEntity_t;
    typedef struct renderView_s renderView_t;
    class RenderWorld;
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
        renderView_t* camera;
        renderEntity_t *renderEntity;
        RenderWorld *renderWorld;
    };
}

#endif
