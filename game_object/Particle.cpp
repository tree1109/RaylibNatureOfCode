#include "Particle.h"

CParticle::CParticle(const Vector2 &position)
    : m_position(position)
{
}

void CParticle::Update()
{
    m_velocity += m_acceleration * GetFrameTime();
    m_position += m_velocity * GetFrameTime();

    --m_lifeTime;
    m_acceleration = Vector2Zeros;
}

void CParticle::Draw() const
{
    const float value = static_cast<float>(m_lifeTime) / 600.0f;
    const auto color = ColorFromHSV(90.0f, 1.0f, value);
    DrawPixelV(m_position, color);
}

void CParticle::ApplyForce(const Vector2 &force)
{
    m_acceleration += force;
}

bool CParticle::IsDead() const
{
    return m_lifeTime < 0;
}
