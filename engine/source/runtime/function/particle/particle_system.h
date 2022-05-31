#pragma once

#include "runtime/function/particle/particle_emitter.h"

#include "runtime/core/math/random.h"
#include "runtime/core/math/vector4.h"

namespace Pilot
{
    struct ParticleRenderInfo
    {
        std::vector<Vector4> positions;
        std::vector<Vector4> scales;
        std::vector<Vector4> colors;
    };

    class ParticleSystem
    {
    public:
        void initialize();
        void tick(float delta_time);

        ParticleRenderInfo getParticleRenderInfo() const;

    private:
        ParticleEmitter m_emitter;
        DefaultRNG      m_random_engine;

        void spawn();
    };
} // namespace Pilot
