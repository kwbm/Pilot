#include "runtime/function/particle/particle_manager.h"

namespace Pilot
{
    void ParticleManager::initialize() {}

    void ParticleManager::spawn(const ParticleSystemSpawnInfo& spawn_info)
    {
        size_t current_emit_count = 0;
        for (const ParticleEmitterSpawnInfo& emitter_info : spawn_info.emitter_spawn_infos)
        {
            current_emit_count += emitter_info.particle_count;
        }

        // if pool is not full
        if (getAliveParticleCount() + current_emit_count <= k_max_particle_count)
        {
            ParticleSystem& system = m_particle_systems.emplace_back();
            for (const ParticleEmitterSpawnInfo& emitter_spawn_info : spawn_info.emitter_spawn_infos)
            {
                system.spawn(emitter_spawn_info);
            }
        }
    }

    void ParticleManager::tick(float delta_time)
    {
        // tick all particle systems
        for (ParticleSystem& system : m_particle_systems)
        {
            system.tick(delta_time);
        }

        // kill dead particle systems
        m_particle_systems.remove_if([](const ParticleSystem& system) { return system.isDead(); });
    }

    size_t ParticleManager::getAliveParticleCount() const
    {
        size_t alive_count = 0;
        for (const ParticleSystem& system : m_particle_systems)
        {
            alive_count += system.getAliveParticleCount();
        }
        return alive_count;
    }

} // namespace Pilot
