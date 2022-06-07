#pragma once

#include "runtime/core/math/vector3.h"

namespace Pilot
{
    struct Particle
    {
        Vector3 position {0.0f, 0.0f, 0.0f};
        Vector3 velocity {0.0f, 0.0f, 0.5f};
        Vector3 acceleration {0.0f, 0.0f, -4.9f};
        Vector3 size {0.03f, 0.03f, 0.03f};
        Vector3 color {1.0f, 1.0f, 1.0f};
        float   lifetime {4.0f};
    };
} // namespace Pilot
