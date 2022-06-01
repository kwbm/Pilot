#include "runtime/function/particle/particle_system.h"

#include "runtime/function/global/global_context.h"
#include "runtime/function/render/render_camera.h"
#include "runtime/function/render/render_system.h"

namespace Pilot
{
    void ParticleSystem::initialize() {}

    void ParticleSystem::spawn(const ParticleEmitterSpawnInfo& spawn_info)
    {
        ParticleEmitter& emitter = m_emitters.emplace_back();
        emitter.spawn(spawn_info);
    }

    void ParticleSystem::tick(float delta_time)
    {
        // simulate all particles in all emitters
        for (ParticleEmitter& emitter : m_emitters)
        {
            ParticleSimulateInfo simulate_info = {delta_time, g_runtime_global_context.m_render_system->getRenderCamera()->position()};
            emitter.simulate(simulate_info);
        }

        // kill dead emitters
        m_emitters.remove_if([](const ParticleEmitter& emitter) { return emitter.isDead(); });

        // append alive emitter render info to logic particle swap data
        RenderSwapContext& render_swap_context = g_runtime_global_context.m_render_system->getSwapContext();
        RenderSwapData&    logic_swap_data     = render_swap_context.getLogicSwapData();

        for (const ParticleEmitter& emitter : m_emitters)
        {
            ParticleRenderInfo&& render_info = emitter.getRenderInfo();
            if (!render_info.positions.empty())
            {
                ParticleSwapData particle_swap_data;
                particle_swap_data.positions = std::move(render_info.positions);
                particle_swap_data.sizes     = std::move(render_info.sizes);
                particle_swap_data.colors    = std::move(render_info.colors);
                logic_swap_data.appendParticleSwapData(particle_swap_data);
            }
        }
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

    size_t ParticleSystem::getAliveParticleCount() const
    {
        size_t alive_count = 0;
        for (const ParticleEmitter& emitter : m_emitters)
        {
            alive_count += emitter.getAliveParticleCount();
        }
        return alive_count;
    }

} // namespace Pilot
