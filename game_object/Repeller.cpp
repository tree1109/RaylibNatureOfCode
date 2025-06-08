#include "Repeller.h"

#include <cmath>
#include <stdexcept>

CRepeller::CRepeller(const Vector2& position, float power) :
    m_position(position),
    m_power(power)
{
}

void CRepeller::Draw() const
{
    DrawCircleV(m_position, std::sqrt(std::abs(m_power)), YELLOW);
}

CRepeller& CRepeller::SetPower(float power)
{
    m_power = power;
    return *this;
}

CRepeller& CRepeller::SetPosition(const Vector2& position)
{
    m_position = position;
    return *this;
}

Vector2 CRepeller::GetForce(CParticle& particle) const
{
    const Vector2 force = m_position - particle.GetPosition();
    const float distance = Clamp(Vector2Length(force), 1.0f, 4096.0f);
    const Vector2 direction = Vector2Normalize(force);
    const float strength = -1.0f * m_power / (distance * distance);

    return Vector2Scale(direction, strength);
}
