#include "runtime/function/particle/particle_emitter.h"

namespace Pilot
{
    ParticleEmitter::ParticleEmitter() {}

    void ParticleEmitter::spawn(const ParticleEmitterSpawnInfo& spawn_info)
    {
        m_spawn_info = spawn_info;

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

        emit();
    }

    void ParticleEmitter::emit()
    {
        std::vector<Particle> particles(m_spawn_info.particle_count);
        for (size_t i = 0; i < particles.size(); i++)
        {
            std::random_device r;
            std::seed_seq      seed {r(), r(), r(), r(), r()};
            m_random_engine.seed(seed);

            float rnd0 = m_random_engine.uniformDistribution<float>(-1.0f, 1.0f);
            float rnd1 = m_random_engine.uniformDistribution<float>(-1.0f, 1.0f);
            float rnd2 = m_random_engine.uniformDistribution<float>(-1.0f, 1.0f);

            particles[i].velocity.x += rnd0 * rnd1 * 0.5f;
            particles[i].velocity.y += rnd1 * rnd2 * 0.5f;
            particles[i].velocity.z += rnd2 * rnd0 * 0.5f;

            particles[i].position.x = rnd0 * m_spawn_info.position_variance.x + m_spawn_info.position_base.x;
            particles[i].position.y = rnd1 * m_spawn_info.position_variance.y + m_spawn_info.position_base.y;
            particles[i].position.z = rnd2 * m_spawn_info.position_variance.z + m_spawn_info.position_base.z;

            particles[i].size.x = rnd0 * m_spawn_info.size_variance.x + m_spawn_info.size_base.x;
            particles[i].size.y = rnd1 * m_spawn_info.size_variance.y + m_spawn_info.size_base.y;
            particles[i].size.z = rnd2 * m_spawn_info.size_variance.z + m_spawn_info.size_base.z;

            particles[i].color.x = (rnd0 + 1.0f) * 0.5f;
            particles[i].color.y = (rnd1 + 1.0f) * 0.5f;
            particles[i].color.z = (rnd2 + 1.0f) * 0.5f;

            particles[i].acceleration.x = rnd0 * m_spawn_info.acceleration_variance.x + m_spawn_info.acceleration_base.x;
            particles[i].acceleration.y = rnd1 * m_spawn_info.acceleration_variance.y + m_spawn_info.acceleration_base.y;
            particles[i].acceleration.z = rnd2 * m_spawn_info.acceleration_variance.z + m_spawn_info.acceleration_base.z;

            particles[i].lifetime += rnd0 * m_spawn_info.lifetime_variance + m_spawn_info.lifetime_base;
        }

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

    void ParticleEmitter::simulate(const ParticleSimulateInfo& simulate_info)
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
                m_alive_particle_count -= 1;

                m_dead_particle_indices[i] = i;
                m_particle_distances[i]    = std::numeric_limits<float>::max();

                continue;
            }

            particle.lifetime -= simulate_info.delta_time;
            particle.velocity += particle.acceleration * simulate_info.delta_time;
            particle.position += particle.velocity * simulate_info.delta_time;

            m_particle_distances[i] = simulate_info.camera.squaredDistance(particle.position);
        }
    }

    bool ParticleEmitter::isDead() const
    {
        for (const Particle& particle : m_particle_pool)
        {
            if (particle.is_active)
            {
                return false;
            }
        }
        return true;
    }

    ParticleRenderInfo ParticleEmitter::getRenderInfo() const
    {
        ParticleRenderInfo render_info;
        for (const auto& particle : m_particle_pool)
        {
            if (particle.is_active)
            {
                Vector4 position = Vector4(particle.position.x, particle.position.y, particle.position.z, 1.0f);
                Vector4 size     = Vector4(particle.size.x, particle.size.y, particle.size.z, 1.0f);
                Vector4 color    = Vector4(particle.color.x, particle.color.y, particle.color.z, 1.0f);
                render_info.positions.push_back(position);
                render_info.sizes.push_back(size);
                render_info.colors.push_back(color);
            }
        }
        return render_info;
    }

    size_t ParticleEmitter::getAliveParticleCount() const { return static_cast<size_t>(m_alive_particle_count); }
} // namespace Pilot
