#ifndef __DEMO_TRIANGLE_H__
#define __DEMO_TRIANGLE_H__

#include "demo.h"

namespace Tutorial {
	class DemoTriangle : public Demo {
	public:
		static const char *s_name;
	protected:
		void OnUpdate();
	};
}

#endif