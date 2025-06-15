#include "RandomField.h"

#include "raymath.h"
#include "utility/Math.h"

CRandomField::CRandomField(const float power)
    : m_power(power)
{
}

void CRandomField::Draw() const
{
}

CRandomField& CRandomField::SetPower(const float power)
{
    m_power = std::abs(power);
    return *this;
}

Vector2 CRandomField::GetForce(CParticle& particle) const
{
    const auto randomDirection = math::GetRandomDirection();
    return Vector2Scale(randomDirection, m_power);
}
