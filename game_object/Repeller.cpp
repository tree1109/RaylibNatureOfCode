#include "Repeller.h"

#include <raymath.h>
#include <cmath>

#include "Particle.h"

CRepeller::CRepeller(const Vector2& position, const float power) :
    m_position(position),
    m_power(power)
{
}

void CRepeller::Draw() const
{
    DrawCircleLinesV(m_position, 10.0f, YELLOW);
}

CRepeller& CRepeller::SetPower(const float power)
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
