#include "runtime/function/particle/particle_system.h"

#include "runtime/function/global/global_context.h"
#include "runtime/function/render/render_camera.h"
#include "runtime/function/render/render_system.h"

namespace Pilot
{
    void ParticleSystem::initialize() { spawn(); }

    void ParticleSystem::tick(float delta_time)
    {
        m_emitter.simulate(delta_time, g_runtime_global_context.m_render_system->getRenderCamera()->position());

        RenderSwapContext& render_swap_context = g_runtime_global_context.m_render_system->getSwapContext();
        RenderSwapData&    logic_swap_data     = render_swap_context.getLogicSwapData();

        ParticleRenderInfo&& particle_render_info = getParticleRenderInfo();
        if (!particle_render_info.positions.empty())
        {
            ParticleSwapData particle_swap_data;
            particle_swap_data.positions = std::move(particle_render_info.positions);
            particle_swap_data.scales    = std::move(particle_render_info.scales);
            particle_swap_data.colors    = std::move(particle_render_info.colors);
            logic_swap_data.appendParticleSwapData(particle_swap_data);
        }
        else
        {
            spawn();
        }
    }

    ParticleRenderInfo ParticleSystem::getParticleRenderInfo() const
    {
        ParticleRenderInfo render_info;
        for (const auto& particle : m_emitter.getParticlePool())
        {
            if (particle.is_active)
            {
                Vector4 position = Vector4(particle.position.x, particle.position.y, particle.position.z, 1.0f);
                Vector4 scale    = Vector4(particle.scale.x, particle.scale.y, particle.scale.z, 1.0f);
                Vector4 color    = Vector4(particle.color.x, particle.color.y, particle.color.z, 1.0f);
                render_info.positions.push_back(position);
                render_info.scales.push_back(scale);
                render_info.colors.push_back(color);
            }
        }
        return render_info;
    }

    void ParticleSystem::spawn()
    {
        const size_t          particle_count = 300;
        std::vector<Particle> particles(particle_count);
        for (size_t i = 0; i < particle_count; i++)
        {
            std::random_device r;
            std::seed_seq      seed {r(), r(), r(), r(), r()};
            m_random_engine.seed(seed);

            float rnd0 = m_random_engine.uniformDistribution<float>(-1.0f, 1.0f);
            float rnd1 = m_random_engine.uniformDistribution<float>(-1.0f, 1.0f);
            float rnd2 = m_random_engine.uniformDistribution<float>(-1.0f, 1.0f);

            particles[i].velocity.x += rnd0 * rnd1;
            particles[i].velocity.y += rnd1 * rnd2;
            particles[i].velocity.z += rnd2 * rnd0 * 4.0f;

            particles[i].position.x += rnd0 * rnd1 * 2.0f;
            particles[i].position.y += rnd1 * rnd2 * 2.0f;
            particles[i].position.z += rnd2 * rnd0 * 4.0f;

            particles[i].color.x = (rnd0 + 1.0f) * 0.5f;
            particles[i].color.y = (rnd1 + 1.0f) * 0.5f;
            particles[i].color.z = (rnd2 + 1.0f) * 0.5f;

            particles[i].scale.x += rnd0 * 0.02f;
            particles[i].scale.y += rnd1 * 0.02f;

            particles[i].lifetime += rnd0 * rnd1 * rnd2 * 4.0f;
        }

        m_emitter.emit(particles);
    }

} // namespace Pilot
