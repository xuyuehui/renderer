#include "demo_triangle.h"
#include "../platform/platform.h"
#include "../renderer/renderer.h"
#include "../math/math.h"
#include "../shared.h"
#include "../utility/utility.h"

using namespace CG;

namespace Tutorial {
	const char *DemoTriangle::s_name = "triangle";

	void DemoTriangle::OnUpdate() {
		app->GetRenderer()->DrawLine(Vec2(50, 50), Vec2(200, 100), color_t(255, 255, 255), -1.0f);
	}
}