#include "runtime/function/particle/particle_emitter.h"
#include "runtime/function/particle/particle_pool.h"

namespace Pilot
{
    void ParticleEmitter::spawn(const ParticleEmitterSpawnInfo& spawn_info, std::shared_ptr<ParticlePool> particle_pool)
    {
        m_particle_pool_ptr = particle_pool;

        for (size_t i = 0; i < spawn_info.particle_count; i++)
        {
            Particle particle;

            std::random_device r;
            std::seed_seq      seed {r(), r(), r(), r(), r()};
            m_random_engine.seed(seed);

            float rnd0 = m_random_engine.uniformDistribution<float>(-1.0f, 1.0f);
            float rnd1 = m_random_engine.uniformDistribution<float>(-1.0f, 1.0f);
            float rnd2 = m_random_engine.uniformDistribution<float>(-1.0f, 1.0f);

            particle.velocity.x = spawn_info.velocity_base.x + rnd0 * spawn_info.velocity_variance.x;
            particle.velocity.y = spawn_info.velocity_base.y + rnd1 * spawn_info.velocity_variance.y;
            particle.velocity.z = spawn_info.velocity_base.z + rnd2 * spawn_info.velocity_variance.z;

            particle.position.x = rnd0 * spawn_info.position_variance.x + spawn_info.position_base.x;
            particle.position.y = rnd1 * spawn_info.position_variance.y + spawn_info.position_base.y;
            particle.position.z = rnd2 * spawn_info.position_variance.z + spawn_info.position_base.z;

            particle.size.x = rnd0 * spawn_info.size_variance.x + spawn_info.size_base.x;
            particle.size.y = rnd1 * spawn_info.size_variance.y + spawn_info.size_base.y;
            particle.size.z = rnd2 * spawn_info.size_variance.z + spawn_info.size_base.z;

            particle.color.x = (rnd0 + 1.0f) * 0.5f;
            particle.color.y = (rnd1 + 1.0f) * 0.5f;
            particle.color.z = (rnd2 + 1.0f) * 0.5f;

            particle.acceleration.x = rnd0 * spawn_info.acceleration_variance.x + spawn_info.acceleration_base.x;
            particle.acceleration.y = rnd1 * spawn_info.acceleration_variance.y + spawn_info.acceleration_base.y;
            particle.acceleration.z = rnd2 * spawn_info.acceleration_variance.z + spawn_info.acceleration_base.z;

            particle.lifetime += rnd0 * spawn_info.lifetime_variance + spawn_info.lifetime_base;

            size_t pool_index;
            particle_pool->emit(particle, pool_index);
            if (pool_index != k_invalid_particle_index)
            {
                m_particle_pool_indices.push_back(pool_index);
            }
        }
    }

    bool ParticleEmitter::isDead() const
    {
        const auto& pool = m_particle_pool_ptr->getParticlePool();
        for (size_t i : m_particle_pool_indices)
        {
            if (pool[i].lifetime > 0.0f)
            {
                return false;
            }
        }
        return true;
    }
} // namespace Pilot
