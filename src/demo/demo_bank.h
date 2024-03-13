#ifndef __DEMO_BANK_H__
#define __DEMO_BANK_H__

#include "demo.h"

namespace CG {

typedef struct renderEntity_s renderEntity_t;
typedef struct renderView_s renderView_t;
class RenderWorld;

}


namespace Tutorial {

class DemoBank : public Demo {
public:
    static const char *s_name;

    DemoBank();
protected:
    void OnInit();
    void OnUpdate();
    void OnShutdown();
private:
    renderView_t *camera;
    renderEntity_t *renderEntity;
    RenderWorld *renderWorld;
};

}

#endif
