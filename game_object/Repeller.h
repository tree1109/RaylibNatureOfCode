#pragma once

#ifndef INCLUDE_REPELLER_H
#define INCLUDE_REPELLER_H

#include <raylib.h>
#include <raymath.h>

#include "IForceField.h"
#include "Particle.h"

class CRepeller : public IForceField
{
public:
    CRepeller(const Vector2& position, float power);
    ~CRepeller() override = default;

    void Draw() const override;

    CRepeller& SetPosition(const Vector2& position);
    CRepeller& SetPower(float power);

    Vector2 GetForce(CParticle& particle) const override;

private:
    Vector2 m_position;
    float m_power = 1.0f;
};

#endif // !INCLUDE_REPELLER_H
