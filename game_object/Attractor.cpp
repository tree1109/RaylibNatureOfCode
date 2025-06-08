#include "Attractor.h"

#include <cmath>
#include <stdexcept>

CAttractor::CAttractor(const Vector2& position, float power) :
    m_position(position),
    m_power(power)
{
}

void CAttractor::Draw() const
{
    DrawCircleV(m_position, std::sqrt(std::abs(m_power)), PURPLE);
}

CAttractor& CAttractor::SetPosition(const Vector2& position)
{
    m_position = position;
    return *this;
}

CAttractor& CAttractor::SetPower(float power)
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
