#pragma once

#ifndef I_FORCE_FIELD_H
#define I_FORCE_FIELD_H

#include <raylib.h>

class CParticle;

class IForceField
{
public:
    virtual ~IForceField() = default;

    virtual void Draw() const = 0;

    virtual Vector2 GetForce(CParticle& particle) const = 0;
};

#endif // !I_FORCE_FIELD_H