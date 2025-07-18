#pragma once

#ifndef INCLUDE_ATTRACTOR_H
#define INCLUDE_ATTRACTOR_H

#include <raylib.h>

#include "IForceField.h"

class CAttractor : public IForceField
{
public:
    CAttractor(const Vector2& position, const float power);
    ~CAttractor() override = default;

    void Draw() const override;

    CAttractor& SetPosition(const Vector2& position);
    CAttractor& SetPower(const float power);

    Vector2 GetForce(CParticle& particle) const override;

private:
    Vector2 m_position;
    float m_power = 1.0f;
};

#endif // !INCLUDE_ATTRACTOR_H
