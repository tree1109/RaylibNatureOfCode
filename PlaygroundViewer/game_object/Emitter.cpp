#include "Emitter.h"

#include "IForceField.h"
#include "utility/Math.h"

#include <algorithm>

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

            if (m_bRandomColor) {
                const Color color = ColorFromHSV(math::GetRandomValue(0.0f, 360.0f), 1.0f, 1.0f);
                m_particles.back().SetColor(color);
            }
        }
    }

    // Update existing particles.
    for (auto& particle : m_particles) {
        const auto randomForce = math::GetRandomGaussianVector2(10.0f);
        const Vector2 netForce = m_force + randomForce;

        // Set color based on life-time remaining.
        {
            const float value = std::max(particle.GetLifeTimeRemaining(), 0.0f) / particle.GetLifeTime();
            const auto tint = ColorAlpha(ColorFromHSV(0.0f, 0.0f, value), std::sqrt(value));
            particle.SetTint(tint);
        }

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
    if (m_bDrawParticleWithTexture) {
        // Draw particles with texture center.
        const Vector2 textureOffset = {
            static_cast<float>(m_particleTexture.width) * -0.5f,
            static_cast<float>(m_particleTexture.height) * -0.5f
        };

        for (const auto& particle : m_particles) {
            const Vector2 texturePosition = particle.GetPosition() + textureOffset;
            DrawTextureV(m_particleTexture, texturePosition, ColorTint(particle.GetColor(), m_tint));
        }
    }
    else {
        for (const auto& particle : m_particles) {
            particle.Draw();
        }
    }
}

CEmitter& CEmitter::ApplyForce(const Vector2& force)
{
    m_force += force;
    return *this;
}

CEmitter& CEmitter::ApplyForceField(const IForceField& forceField)
{
    for (auto& particle : m_particles) {
        const Vector2 force = forceField.GetForce(particle);
        particle.ApplyForce(force);
    }
    return *this;
}
CEmitter& CEmitter::SetTint(const Color& tint)
{
    m_tint = tint;
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
CEmitter& CEmitter::SetRandomColor(const bool isRandomColor)
{
    m_bRandomColor = isRandomColor;
    return *this;
}

CEmitter& CEmitter::SetDrawParticleWithTexture(const bool isDawParticleWithTexture)
{
    m_bDrawParticleWithTexture = isDawParticleWithTexture;
    return *this;
}

CEmitter& CEmitter::SetParticleTexture(const Texture2D& texture)
{
    m_particleTexture = texture;
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
