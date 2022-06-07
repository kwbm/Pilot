#pragma once

#include "runtime/function/particle/particle.h"
#include "runtime/function/particle/particle_common.h"

#include "runtime/core/math/random.h"

#include <cstdint>
#include <vector>

namespace Pilot
{
    class ParticlePool;

    struct ParticleEmitterSpawnInfo
    {
        size_t  particle_count;
        Vector3 position_base;
        Vector3 position_variance;
        Vector3 velocity_base;
        Vector3 velocity_variance;
        Vector3 acceleration_base;
        Vector3 acceleration_variance;
        Vector3 size_base;
        Vector3 size_variance;
        float   lifetime_base;
        float   lifetime_variance;
    };

    class ParticleEmitter
    {
    public:
        void spawn(const ParticleEmitterSpawnInfo& spawn_info, std::shared_ptr<ParticlePool> particle_pool);
        bool isDead() const;

    private:
        DefaultRNG m_random_engine;

        std::vector<size_t>           m_particle_pool_indices;
        std::shared_ptr<ParticlePool> m_particle_pool_ptr;
    };
} // namespace Pilot
