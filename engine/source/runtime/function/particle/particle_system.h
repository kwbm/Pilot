#pragma once

#include "runtime/function/particle/particle_common.h"
#include "runtime/function/particle/particle_emitter.h"

#include <list>

namespace Pilot
{
    class ParticlePool;

    struct ParticleSystemSpawnInfo
    {
        std::vector<ParticleEmitterSpawnInfo> emitter_spawn_infos;
    };

    class ParticleSystem
    {
    public:
        void spawn(const ParticleEmitterSpawnInfo& spawn_info, std::shared_ptr<ParticlePool> particle_pool);
        void tick(float delta_time);
        bool isDead() const;

    private:
        std::list<ParticleEmitter> m_emitters;
    };
} // namespace Pilot
