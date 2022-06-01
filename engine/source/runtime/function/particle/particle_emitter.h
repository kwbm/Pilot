#pragma once

#include "runtime/function/particle/particle.h"
#include "runtime/function/particle/particle_common.h"

#include "runtime/core/math/random.h"

#include <cstdint>
#include <vector>

namespace Pilot
{
    struct ParticleEmitterSpawnInfo
    {
        size_t  particle_count;
        Vector3 position_base;
        Vector3 position_variance;
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
        ParticleEmitter();

        void spawn(const ParticleEmitterSpawnInfo& spawn_info);
        void emit();
        void simulate(const ParticleSimulateInfo& simulate_info);
        bool isDead() const;

        ParticleRenderInfo getRenderInfo() const;
        size_t             getAliveParticleCount() const;

    private:
        ParticleEmitterSpawnInfo m_spawn_info;
        DefaultRNG               m_random_engine;

        std::vector<Particle> m_particle_pool;
        std::vector<size_t>   m_dead_particle_indices;
        std::vector<size_t>   m_active_particle_lists[2];
        std::vector<float>    m_particle_distances;

        uint8_t m_active_list_index {0};
        uint8_t m_process_list_index {1};

        int32_t m_alive_particle_count {0};
        int32_t m_dead_particle_count {0};
        int32_t m_real_emit_count {0};
        int32_t m_alive_count_after_simulation {0};
    };
} // namespace Pilot
