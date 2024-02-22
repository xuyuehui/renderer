#include <vector>
#include <iostream>

#include "platform/platform.h"
#include "demo/demo.h"

using namespace CG;
using namespace Tutorial;

int main(int argc, char **argv) {
    Application *app = InitializeApplication(argc, argv);
    Demo *demo = InitializeDemo(argc, argv);

    return demo->Run(app);
}