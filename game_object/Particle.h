#pragma once

#ifndef INCLUDER_PARTICLE_H
#define INCLUDER_PARTICLE_H

#include <raylib.h>
#include <raymath.h>
#include <cstdint>

class CParticle
{
public:
    CParticle(const Vector2& position);

    void Update();
    void Draw() const;

    void ApplyForce(const Vector2& force);

    [[nodiscard]] bool IsDead() const;

private:
    Vector2 m_position = Vector2Zeros;
    Vector2 m_velocity = Vector2Zeros;
    Vector2 m_acceleration = Vector2Zeros;
    int32_t m_lifeTime = 600;
};

#endif //!INCLUDER_PARTICLE_H
