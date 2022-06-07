#include "runtime/function/particle/particle_pool.h"

#include <utility>

namespace Pilot
{
    ParticlePool::ParticlePool()
    {
        m_particle_pool.resize(k_max_particle_count);
        m_alive_list_current.resize(k_max_particle_count);
        m_alive_list_new.resize(k_max_particle_count);
        m_dead_list.resize(k_max_particle_count);
        for (size_t i = 0; i < m_dead_list.size(); i++)
        {
            m_dead_list[i] = i;
        }

        m_after_culling_list0.resize(k_max_particle_count);
        m_after_culling_list1.resize(k_max_particle_count);

        m_distance_buffer.resize(k_max_particle_count);

        m_particle_counters.alive_count                  = 0;
        m_particle_counters.dead_count                   = k_max_particle_count;
        m_particle_counters.real_emit_count              = 0;
        m_particle_counters.alive_count_after_simulation = 0;
        m_particle_counters.alive_count_after_culling    = 0;
    }

    const std::vector<Particle>& ParticlePool::getParticlePool() const { return m_particle_pool; }

    void ParticlePool::preSpawn(size_t emit_count)
    {
        m_current_frame_emit_count = 0;

        size_t dead_count                                = m_particle_counters.dead_count;
        size_t pre_alive_count                           = m_particle_counters.alive_count_after_simulation;
        m_particle_counters.real_emit_count              = std::min(dead_count, emit_count);
        m_particle_counters.alive_count                  = pre_alive_count;
        m_particle_counters.alive_count_after_simulation = 0;
        m_particle_counters.alive_count_after_culling    = 0;
    }

    void ParticlePool::emit(const Particle& particle, size_t& pool_index)
    {
        pool_index = k_invalid_particle_index;
        if (m_current_frame_emit_count < m_particle_counters.real_emit_count)
        {
            size_t dead_count;
            interlockedAdd(m_particle_counters.dead_count, -1, dead_count);
            size_t new_particle_index = m_dead_list[dead_count - 1];

            m_particle_pool[new_particle_index] = particle;
            pool_index                          = new_particle_index;

            size_t alive_count;
            interlockedAdd(m_particle_counters.alive_count, 1, alive_count);
            m_alive_list_current[alive_count] = new_particle_index;
        }
        m_current_frame_emit_count++;
    }

    void ParticlePool::simulate(const ParticleSimulateInfo& simulate_info)
    {
        for (size_t i = 0; i < m_particle_counters.alive_count; i++)
        {
            size_t    particle_index = m_alive_list_current[i];
            Particle& particle       = m_particle_pool[particle_index];

            if (particle.lifetime > 0.0f)
            {
                particle.lifetime -= simulate_info.delta_time;
                particle.velocity += particle.acceleration * simulate_info.delta_time;
                particle.position += particle.velocity * simulate_info.delta_time;

                size_t new_alive_index;
                interlockedAdd(m_particle_counters.alive_count_after_simulation, 1, new_alive_index);
                m_alive_list_new[new_alive_index] = particle_index;

                bool culled = false; // TODO frustum culling
                if (!culled)
                {
                    size_t prev_after_culling_count;
                    interlockedAdd(m_particle_counters.alive_count_after_culling, 1, prev_after_culling_count);
                    m_after_culling_list0[prev_after_culling_count] = prev_after_culling_count;
                    m_after_culling_list1[prev_after_culling_count] = particle_index;

                    // write to distance buffer
                    m_distance_buffer[prev_after_culling_count] = simulate_info.camera.squaredDistance(particle.position);
                }
            }
            else
            {
                size_t dead_index;
                interlockedAdd(m_particle_counters.dead_count, 1, dead_index);
                m_dead_list[dead_index] = particle_index;
            }
        }
    }

    void ParticlePool::sort()
    {
        // TODO
    }

    void ParticlePool::swapAliveList() { std::swap(m_alive_list_current, m_alive_list_new); }

    ParticleRenderInfo ParticlePool::getRenderInfo() const
    {
        ParticleRenderInfo render_info;
        for (size_t i = 0; i < m_particle_counters.alive_count_after_culling; i++)
        {
            size_t culling_list_index = m_after_culling_list0[i];
            size_t particle_index     = m_after_culling_list1[culling_list_index];

            const Particle& particle = m_particle_pool[particle_index];

            Vector4 position = Vector4(particle.position.x, particle.position.y, particle.position.z, 1.0f);
            Vector4 size     = Vector4(particle.size.x, particle.size.y, particle.size.z, 1.0f);
            Vector4 color    = Vector4(particle.color.x, particle.color.y, particle.color.z, 1.0f);
            render_info.positions.push_back(position);
            render_info.sizes.push_back(size);
            render_info.colors.push_back(color);
        }
        return render_info;
    }

    void ParticlePool::interlockedAdd(size_t& counter, int value, size_t& origin)
    {
        origin = counter;
        counter += value;
    }
} // namespace Pilot
