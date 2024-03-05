#ifndef __DEMO_TRIANGLE_H__
#define __DEMO_TRIANGLE_H__

#include "demo.h"

namespace CG {
    typedef struct renderEntity_s renderEntity_t;
    typedef struct renderView_s renderView_t;
    class RenderWorld;
}

namespace Tutorial {
    class DemoTriangle : public Demo {
    public:
        static const char *s_name;
        
        DemoTriangle();
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
