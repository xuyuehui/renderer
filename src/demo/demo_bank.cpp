#include "demo_bank.h"
#include "../platform/platform.h"
#include "../renderer/renderer.h"
#include "../math/maths.h"
#include "../math/vector.h"
#include "../math/matrix.h"
#include "../math/quat.h"

using namespace CG;
using namespace std;

namespace Tutorial {
    const char *DemoBank::s_name = "blank";

    void DemoBank::OnInit() {
        Mat4 projMat = Math::ProjectMatrix(PI / 3, 1.0f, 0.1f, 1000.0f);
        Mat4 viewMat = Math::ViewMatrix(Vec3(.0f, .0f, -3.0f), Vec3(.0f, .0f, .0f), Vec3(.0f, 1.0f, 0.0f));
        Mat4 modelMat = Math::FromRTS(Vec3(0.0f, 0.0f, 10.0f), Quat::Indentity(), Vec3(1.0f, 1.0f, 1.0f));
        
        Mat4 mvpMat = projMat * viewMat * modelMat;
        
        Vec4 v1(1.000000f, -1.000000f, 100.000000f, 1.0f);
        Vec4 v2(-1.000000f, -1.000000f, 0.0001f, 1.0f);
        Vec4 v3(-1.000000f, -1.000000f, 500.000000f, 1.0f);
        
        Vec4 tv1 = projMat * viewMat * modelMat * v1;
        Vec4 tv2 = projMat * viewMat * modelMat * v2;
        Vec4 tv3 = projMat * viewMat * modelMat * v3;
        
        Math::PerspectiveDivision(tv1);
        Math::PerspectiveDivision(tv2);
        Math::PerspectiveDivision(tv3);
        
        Vec3 e1 = (v2 - v1).ToVec3();
        Vec3 e2 = (v3 - v1).ToVec3();
        
        bool backface = e1.Cross(e2).z < 0.0f;
        
        printf("v1 (%.2f, %.2f, %.5f, %.2f) -> (%.2f, %.2f, %.5f, %.2f)\n", v1.x, v1.y, v1.z, v1.w, tv1.x, tv1.y, tv1.z, tv1.w);
        printf("v2 (%.2f, %.2f, %.5f, %.2f) -> (%.2f, %.2f, %.5f, %.2f)\n", v2.x, v2.y, v2.z, v2.w, tv2.x, tv2.y, tv2.z, tv2.w);
        printf("v3 (%.2f, %.2f, %.5f, %.2f) -> (%.2f, %.2f, %.5f, %.2f)\n", v3.x, v3.y, v3.z, v3.w, tv3.x, tv3.y, tv3.z, tv3.w);
        
        printf("is backface? %s\n", backface ? "true" : "false");
    }

    void DemoBank::OnUpdate(){
        cout << "[DemoBlank] OnUpdate" << endl;

        app->GetRenderer()->DrawText("hello world 1234", Vec2(100, 100), 6, color_t(255, 0, 0), 0.5f);
    }
}
