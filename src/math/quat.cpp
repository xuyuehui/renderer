#include "quat.h"

namespace CG {
    Quat Quat::Indentity() {
        Quat quat(0, 0, 0, 1.0f);
        return quat;
    }
}
