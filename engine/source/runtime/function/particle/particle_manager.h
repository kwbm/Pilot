#pragma once

#include "runtime/function/particle/particle_system.h"

#include <list>

namespace Pilot
{
    class ParticleManager
    {
    public:
        void initialize();

        void spawn(const ParticleSystemSpawnInfo& spawn_info);
        void tick(float delta_time);

    private:
        std::list<ParticleSystem> m_particle_systems;

        size_t getAliveParticleCount() const;
    };
} // namespace Pilot
