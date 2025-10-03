#pragma once

#ifndef INCLUDE_REPELLER_H
#define INCLUDE_REPELLER_H

#include <raylib.h>

#include "IForceField.h"

class CRepeller : public IForceField
{
public:
    CRepeller(const Vector2& position, const float power);
    ~CRepeller() override = default;

    void Draw() const override;

    CRepeller& SetPosition(const Vector2& position);
    CRepeller& SetPower(const float power);

    Vector2 GetForce(CParticle& particle) const override;

private:
    Vector2 m_position;
    float m_power = 1.0f;
};

#endif // !INCLUDE_REPELLER_H
