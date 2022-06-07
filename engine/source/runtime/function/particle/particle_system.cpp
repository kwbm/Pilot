#include "runtime/function/particle/particle_system.h"
#include "runtime/function/particle/particle_pool.h"

#include "runtime/function/global/global_context.h"
#include "runtime/function/render/render_camera.h"
#include "runtime/function/render/render_system.h"

namespace Pilot
{
    void ParticleSystem::spawn(const ParticleEmitterSpawnInfo& spawn_info, std::shared_ptr<ParticlePool> particle_pool)
    {
        ParticleEmitter& emitter = m_emitters.emplace_back();
        emitter.spawn(spawn_info, particle_pool);
    }

    void ParticleSystem::tick(float delta_time)
    {
        // kill dead emitters
        m_emitters.remove_if([](const ParticleEmitter& emitter) { return emitter.isDead(); });
    }

    bool ParticleSystem::isDead() const
    {
        for (const ParticleEmitter& emitter : m_emitters)
        {
            if (!emitter.isDead())
            {
                return false;
            }
        }
        return true;
    }
} // namespace Pilot
