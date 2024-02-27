#include "demo_bank.h"
#include "../platform/platform.h"
#include "../renderer/renderer.h"
#include "../math/maths.h"

using namespace CG;
using namespace std;

namespace Tutorial {
    const char *DemoBank::s_name = "blank";

    void DemoBank::OnUpdate(){
        cout << "[DemoBlank] OnUpdate" << endl;

        app->GetRenderer()->DrawText("hello world 1234", Vec2(100, 100), 6, color_t(255, 0, 0), 0.5f);
    }
}