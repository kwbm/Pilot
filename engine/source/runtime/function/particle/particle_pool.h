#pragma once

#include "runtime/function/particle/particle.h"
#include "runtime/function/particle/particle_common.h"

namespace Pilot
{
    struct ParticleCounters
    {
        size_t alive_count;
        size_t dead_count;
        size_t real_emit_count;
        size_t alive_count_after_simulation;
        size_t alive_count_after_culling;
    };

    class ParticlePool
    {
    public:
        ParticlePool();

        const std::vector<Particle>& getParticlePool() const;

        void preSpawn(size_t emit_count);
        void emit(const Particle& particle, size_t& pool_index);
        void simulate(const ParticleSimulateInfo& simulate_info);
        void sort();
        void swapAliveList();

        ParticleRenderInfo getRenderInfo() const;

    private:
        std::vector<Particle> m_particle_pool;

        ParticleCounters    m_particle_counters;
        std::vector<size_t> m_alive_list_current;
        std::vector<size_t> m_alive_list_new;
        std::vector<size_t> m_dead_list;

        std::vector<size_t> m_after_culling_list0;
        std::vector<size_t> m_after_culling_list1;

        std::vector<float> m_distance_buffer;

        size_t m_current_frame_emit_count {0};

        void interlockedAdd(size_t& counter, int value, size_t& origin);
    };
} // namespace Pilot
