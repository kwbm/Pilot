#include "runtime/function/particle/particle_manager.h"

namespace Pilot
{
    void ParticleManager::initialize()
    {
        m_particle_systems.resize(1);
        for (ParticleSystem& system : m_particle_systems)
        {
            system.initialize();
        }
    }

    void ParticleManager::tick(float delta_time)
    {
        for (ParticleSystem& system : m_particle_systems)
        {
            system.tick(delta_time);
        }
    }

    ParticleRenderInfo ParticleManager::getParticleRenderInfo() const
    {
        ParticleRenderInfo render_info;
        for (const ParticleSystem& system : m_particle_systems)
        {
            ParticleRenderInfo&& system_render_info = system.getParticleRenderInfo();

            render_info.positions.insert(
                render_info.positions.end(), system_render_info.positions.begin(), system_render_info.positions.end());
            render_info.colors.insert(
                render_info.colors.end(), system_render_info.colors.begin(), system_render_info.colors.end());
            render_info.scales.insert(
                render_info.scales.end(), system_render_info.scales.begin(), system_render_info.scales.end());
        }
        return render_info;
    }
} // namespace Pilot
