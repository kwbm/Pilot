#pragma once

#include "runtime/function/particle/particle_system.h"

#include <vector>

namespace Pilot
{
    class ParticleManager
    {
    public:
        void initialize();
        void tick(float delta_time);

        ParticleRenderInfo getParticleRenderInfo() const;

    private:
        std::vector<ParticleSystem> m_particle_systems;
    };
} // namespace Pilot
