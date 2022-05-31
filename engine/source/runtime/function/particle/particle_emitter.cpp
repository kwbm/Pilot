#include "runtime/function/particle/particle_emitter.h"

namespace Pilot
{
    ParticleEmitter::ParticleEmitter()
    {
        m_particle_pool.resize(k_max_particle_count);
        m_dead_particle_indices.resize(k_max_particle_count);
        m_active_particle_lists[0].resize(k_max_particle_count);
        m_active_particle_lists[1].resize(k_max_particle_count);
        m_particle_distances.resize(k_max_particle_count);

        m_dead_particle_count = m_dead_particle_indices.size();
        for (size_t i = 0; i < m_dead_particle_count; i++)
        {
            m_dead_particle_indices[i] = i;
        }
    }

    void ParticleEmitter::emit(const std::vector<Particle>& particles)
    {
        size_t particle_limit = std::min(m_dead_particle_indices.size(), particles.size());
        for (size_t i = 0; i < particle_limit; i++)
        {
            m_particle_pool[i]           = particles[i];
            m_particle_pool[i].is_active = true;

            m_dead_particle_indices[i] = k_invalid_particle_index;
            m_dead_particle_count -= 1;

            m_alive_particle_count += 1;
            m_real_emit_count += 1;
        }
    }

    void ParticleEmitter::simulate(float delta_time, const Vector3& camera)
    {
        for (size_t i = 0; i < m_particle_pool.size(); i++)
        {
            Particle& particle = m_particle_pool[i];

            if (!particle.is_active)
            {
                m_particle_distances[i]    = std::numeric_limits<float>::max();
                m_dead_particle_indices[i] = i;

                continue;
            }

            if (particle.lifetime <= 0.0f)
            {
                particle.is_active = false;
                m_dead_particle_count += 1;
                m_real_emit_count -= 1;

                m_dead_particle_indices[i] = i;
                m_particle_distances[i]    = std::numeric_limits<float>::max();

                continue;
            }

            particle.lifetime -= delta_time;
            particle.velocity += particle.acceleration * delta_time;
            particle.position += particle.velocity * delta_time;

            m_particle_distances[i] = camera.squaredDistance(particle.position);
        }
    }

    const std::vector<Particle>& ParticleEmitter::getParticlePool() const { return m_particle_pool; }
} // namespace Pilot
