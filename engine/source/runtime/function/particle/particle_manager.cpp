#include "runtime/function/particle/particle_manager.h"

#include "runtime/function/global/global_context.h"
#include "runtime/function/render/render_camera.h"
#include "runtime/function/render/render_system.h"

namespace Pilot
{
    void ParticleManager::initialize() { m_particle_pool = std::make_shared<ParticlePool>(); }

    void ParticleManager::spawn(const ParticleSystemSpawnInfo& spawn_info)
    {
        size_t current_emit_count = 0;
        for (const ParticleEmitterSpawnInfo& emitter_info : spawn_info.emitter_spawn_infos)
        {
            current_emit_count += emitter_info.particle_count;
        }

        m_particle_spawn_infos.push(spawn_info);
    }

    void ParticleManager::tick(float delta_time)
    {
        m_particle_update_interval += delta_time;
        if (m_particle_update_interval < 0.01667f)
        {
            return;
        }

        if (!m_particle_spawn_infos.empty())
        {
            const ParticleSystemSpawnInfo& spawn_info = m_particle_spawn_infos.front();

            size_t current_emit_count = 0;
            for (const ParticleEmitterSpawnInfo& emitter_info : spawn_info.emitter_spawn_infos)
            {
                current_emit_count += emitter_info.particle_count;
            }

            m_particle_pool->preSpawn(current_emit_count);

            ParticleSystem& system = m_particle_systems.emplace_back();
            for (const ParticleEmitterSpawnInfo& emitter_spawn_info : spawn_info.emitter_spawn_infos)
            {
                system.spawn(emitter_spawn_info, m_particle_pool);
            }

            m_particle_spawn_infos.pop();
        }
        else
        {
            m_particle_pool->preSpawn(0);
        }

        // simulate alive particles in pool
        ParticleSimulateInfo simulate_info = {m_particle_update_interval, g_runtime_global_context.m_render_system->getRenderCamera()->position()};
        m_particle_pool->simulate(simulate_info);

        m_particle_pool->sort();

        // tick all particle systems
        for (ParticleSystem& system : m_particle_systems)
        {
            system.tick(m_particle_update_interval);
        }

        // kill dead particle systems
        m_particle_systems.remove_if([](const ParticleSystem& system) { return system.isDead(); });

        // append after culling alive particles to logic particle swap data
        RenderSwapContext& render_swap_context = g_runtime_global_context.m_render_system->getSwapContext();
        RenderSwapData&    logic_swap_data     = render_swap_context.getLogicSwapData();

        ParticleRenderInfo&& render_info = m_particle_pool->getRenderInfo();
        if (!render_info.positions.empty())
        {
            ParticleSwapData particle_swap_data;
            particle_swap_data.positions = std::move(render_info.positions);
            particle_swap_data.sizes     = std::move(render_info.sizes);
            particle_swap_data.colors    = std::move(render_info.colors);
            logic_swap_data.appendParticleSwapData(particle_swap_data);
        }

        m_particle_pool->swapAliveList();

        m_particle_update_interval = 0.0f;
    }
} // namespace Pilot
