#include "Emitter.h"

#include "utility/Math.h"

CEmitter::CEmitter(const Vector2& position) :
    m_position(position) {}

void CEmitter::Update()
{
    // Remove dead particles.
    m_particles.erase(
        std::ranges::remove_if(m_particles, [](const CParticle& particle) { return particle.IsDead(); }).begin(),
        m_particles.end());

    // Spawn new particles.
    if (m_bEmitting) {
        for (int32_t i = 0; i < m_particleSpawnCountPerFrame; i++) {
            // Break if the maximum particle count is reached.
            if (m_particles.size() >= m_maxParticleCount) { break; }

            m_particles.emplace_back(m_position);
            m_particles.back()
                       .SetLifeTime(m_particleLifeTime)
                       .ApplyForce(m_initialForce);
        }
    }

    // Update existing particles.
    for (auto& particle : m_particles) {
        const auto randomForce = math::GetRandomGaussianVector2(10.0f);
        const Vector2 netForce = m_force + randomForce;

        particle.ApplyForce(netForce);
        particle.Update();
    }

    m_force = Vector2Zero();
}

void CEmitter::Draw() const
{
    // Draw the emitter position.
    DrawCircleLinesV(m_position, 5.0f, RED);

    // Particle drawing.
    for (const auto& particle : m_particles) {
        particle.Draw();
    }
}

CEmitter& CEmitter::ApplyForce(const Vector2& force)
{
    m_force += force;
    return *this;
}

CEmitter& CEmitter::SetPosition(const Vector2& position)
{
    m_position = position;
    return *this;
}

CEmitter& CEmitter::SetParticleInitialForce(const Vector2& force)
{
    m_initialForce = force;
    return *this;
}

CEmitter& CEmitter::SetParticleLifeTime(const float lifeTime)
{
    constexpr float MinParticleLifeTime = 0.1f;
    m_particleLifeTime = std::max(lifeTime, MinParticleLifeTime);
    return *this;
}

CEmitter& CEmitter::SetParticleSpawnCountPerFrame(const int32_t count)
{
    m_particleSpawnCountPerFrame = std::max(count, 1);
    return *this;
}

CEmitter& CEmitter::SetMaxParticleCount(const int32_t maxCount)
{
    m_maxParticleCount = std::max(maxCount, 1);
    return *this;
}

CEmitter& CEmitter::SetEmitting(const bool isEmitting)
{
    m_bEmitting = isEmitting;
    return *this;
}

size_t CEmitter::GetParticleCount() const
{
    return m_particles.size();
}

size_t CEmitter::GetCapacity() const
{
    return m_particles.capacity();
}
