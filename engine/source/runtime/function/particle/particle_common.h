#pragma once

#include "runtime/core/math/vector3.h"
#include "runtime/core/math/vector4.h"

#include <limits>
#include <vector>

namespace Pilot
{
    static const size_t k_max_particle_count     = 4096;
    static const size_t k_invalid_particle_index = std::numeric_limits<std::size_t>::max();

    struct ParticleSimulateInfo
    {
        float   delta_time;
        Vector3 camera;
    };

    struct ParticleRenderInfo
    {
        std::vector<Vector4> positions;
        std::vector<Vector4> sizes;
        std::vector<Vector4> colors;
    };
} // namespace Pilot
