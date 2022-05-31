#pragma once

#include "runtime/function/particle/particle.h"

#include <cstdint>
#include <limits>
#include <vector>

namespace Pilot
{
    static const size_t k_max_particle_count     = 4096;
    static const size_t k_invalid_particle_index = std::numeric_limits<std::size_t>::max();

    class ParticleEmitter
    {
    public:
        ParticleEmitter();

        void emit(const std::vector<Particle>& particles);
        void simulate(float delta_time, const Vector3& camera);

        const std::vector<Particle>& getParticlePool() const;

    private:
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
