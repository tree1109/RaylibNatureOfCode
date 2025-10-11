#pragma once

#ifndef INCLUDE_MOVER_H
#define INCLUDE_MOVER_H

#include "raylib.h"

class CMover
{
public:
    CMover() = default;
    explicit CMover(const Vector2& position);
    ~CMover() = default;

public:
    void Update();
    void Draw() const;

    const Vector2& GetPosition() const;
    const Vector2& GetVelocity() const;
    const Vector2& GetAcceleration() const;
    float GetRotation() const;

    void SetPosition(const Vector2& position);
    void SetVelocity(const Vector2& velocity);
    void SetAcceleration(const Vector2& acceleration);
    void SetRotation(float rotation);
    void ApplyForce(const Vector2& force);

private:
    Vector2 m_position = {};
    Vector2 m_velocity = {};
    Vector2 m_acceleration = {};
    float m_rotation = 0.0f; // In radian.
};

#endif // INCLUDE_MOVER_H
