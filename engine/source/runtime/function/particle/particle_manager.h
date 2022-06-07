#pragma once

#include "runtime/function/particle/particle_pool.h"
#include "runtime/function/particle/particle_system.h"

#include <list>
#include <memory>
#include <queue>

namespace Pilot
{
    class ParticleManager
    {
    public:
        void initialize();
        void spawn(const ParticleSystemSpawnInfo& spawn_info);
        void tick(float delta_time);

    private:
        std::queue<ParticleSystemSpawnInfo> m_particle_spawn_infos;
        std::list<ParticleSystem>           m_particle_systems;
        std::shared_ptr<ParticlePool>       m_particle_pool;

        float m_particle_update_interval {0.0f};
    };
} // namespace Pilot
