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
    DrawPixelV(m_position, ColorTint(m_color, m_tint));
}

CParticle& CParticle::ApplyForce(const Vector2& force)
{
    m_acceleration += force / m_mass;
    return *this;
}

CParticle& CParticle::SetColor(const Color& color)
{
    m_color = color;
    return *this;
}

CParticle& CParticle::SetTint(const Color& color)
{
    m_tint = color;
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
Color CParticle::GetColor() const
{
    return ColorTint(m_color, m_tint);
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

float CParticle::GetLifeTime() const
{
    return m_lifeTime;
}

float CParticle::GetLifeTimeRemaining() const
{
    return m_lifeTimeRemaining;
}

bool CParticle::IsDead() const
{
    return m_lifeTimeRemaining < 0;
}
