#pragma once

#include "runtime/function/particle/particle_common.h"
#include "runtime/function/particle/particle_emitter.h"

#include <list>

namespace Pilot
{
    struct ParticleSystemSpawnInfo
    {
        std::vector<ParticleEmitterSpawnInfo> emitter_spawn_infos;
    };

    class ParticleSystem
    {
    public:
        void initialize();

        void spawn(const ParticleEmitterSpawnInfo& spawn_info);
        void tick(float delta_time);
        bool isDead() const;

        size_t getAliveParticleCount() const;

    private:
        std::list<ParticleEmitter> m_emitters;
    };
} // namespace Pilot
