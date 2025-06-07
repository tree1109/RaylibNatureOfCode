#pragma once

#ifndef INCLUDER_PARTICLE_H
#define INCLUDER_PARTICLE_H

#include <raylib.h>
#include <raymath.h>

class CParticle
{
public:
    CParticle(const Vector2 &position);

    void Update();
    void Draw() const;

    CParticle &ApplyForce(const Vector2 &force);
    CParticle &SetLifeTime(const float &lifeTime);

    [[nodiscard]] bool IsDead() const;

private:
    Vector2 m_position = Vector2Zeros;
    Vector2 m_velocity = Vector2Zeros;
    Vector2 m_acceleration = Vector2Zeros;
    float m_lifeTime = 10.0f;
    float m_lifeTimeRemaining = 10.0f;
};

#endif //!INCLUDER_PARTICLE_H
