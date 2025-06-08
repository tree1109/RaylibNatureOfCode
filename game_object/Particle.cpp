#include "Particle.h"

#include <algorithm>
#include <stdexcept>

CParticle::CParticle(const Vector2& position) :
    m_position(position)
{
}

void CParticle::Update()
{
    m_velocity += m_acceleration * GetFrameTime();
    m_position += m_velocity * GetFrameTime();

    m_lifeTimeRemaining -= GetFrameTime();
    m_acceleration = Vector2Zeros;
}

void CParticle::Draw() const
{
    const float value = std::max(m_lifeTimeRemaining, 0.0f) / m_lifeTime;
    const auto color = ColorFromHSV(90.0f, 1.0f, value);
    DrawPixelV(m_position, color);
}

CParticle& CParticle::ApplyForce(const Vector2& force)
{
    m_acceleration += force / m_mass;
    return *this;
}

CParticle& CParticle::SetPosition(const Vector2& position)
{
    m_position = position;
    return *this;
}

CParticle& CParticle::SetMass(const float mass)
{
    constexpr float minMass = 0.001f;
    m_mass = std::max(mass, minMass);
    return *this;
}

CParticle& CParticle::SetLifeTime(const float& lifeTime)
{
    m_lifeTime = lifeTime;
    m_lifeTimeRemaining = lifeTime;
    return *this;
}

Vector2 CParticle::GetPosition() const
{
    return m_position;
}

Vector2 CParticle::GetVelocity() const
{
    return m_velocity;
}

Vector2 CParticle::GetAcceleration() const
{
    return m_acceleration;
}

float CParticle::GetMass() const
{
    return m_mass;
}

bool CParticle::IsDead() const
{
    return m_lifeTimeRemaining < 0;
}
