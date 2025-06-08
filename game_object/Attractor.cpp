#include "Attractor.h"

#include <raymath.h>
#include <cmath>

#include "Particle.h"

CAttractor::CAttractor(const Vector2& position, const float power) :
    m_position(position),
    m_power(power)
{
}

void CAttractor::Draw() const
{
    DrawCircleLinesV(m_position, 10.0f, PURPLE);
}

CAttractor& CAttractor::SetPosition(const Vector2& position)
{
    m_position = position;
    return *this;
}

CAttractor& CAttractor::SetPower(const float power)
{
    m_power = power;
    return *this;
}

Vector2 CAttractor::GetForce(CParticle& particle) const
{
    const Vector2 force = m_position - particle.GetPosition();
    const float distance = Clamp(Vector2Length(force), 1.0f, 4096.0f);
    const Vector2 direction = Vector2Normalize(force);
    const float strength = m_power / (distance * distance);

    return Vector2Scale(direction, strength);
}
