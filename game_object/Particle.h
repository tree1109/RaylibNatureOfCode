#pragma once

#ifndef INCLUDE_PARTICLE_H
#define INCLUDE_PARTICLE_H

#include <raylib.h>
#include <raymath.h>

class CParticle
{
public:
    explicit CParticle(const Vector2& position);

    void Update();
    void Draw() const;

    CParticle& ApplyForce(const Vector2& force);
    CParticle& SetColor(const Color& color);
    CParticle& SetTint(const Color& color);
    CParticle& SetPosition(const Vector2& position);
    CParticle& SetMass(const float mass);
    CParticle& SetLifeTime(const float& lifeTime);

    [[nodiscard]] Color GetColor() const;
    [[nodiscard]] Vector2 GetPosition() const;
    [[nodiscard]] Vector2 GetVelocity() const;
    [[nodiscard]] Vector2 GetAcceleration() const;
    [[nodiscard]] float GetMass() const;
    [[nodiscard]] float GetLifeTime() const;
    [[nodiscard]] float GetLifeTimeRemaining() const;
    [[nodiscard]] bool IsDead() const;

private:
    Color m_color = WHITE;
    Color m_tint = WHITE;
    Vector2 m_position = Vector2Zeros;
    Vector2 m_velocity = Vector2Zeros;
    Vector2 m_acceleration = Vector2Zeros;
    float m_mass = 1.0f;
    float m_lifeTime = 10.0f;
    float m_lifeTimeRemaining = 10.0f;
};

#endif //!INCLUDE_PARTICLE_H
