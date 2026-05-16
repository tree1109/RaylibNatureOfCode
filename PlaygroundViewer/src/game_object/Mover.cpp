//
// Created by 莊瞻 on 2025/10/11.
//

#include "Mover.h"

#include <algorithm>
#include <cmath>

#include "raymath.h"

CMover::CMover(const Vector2& position) : m_position(position)
{
}

void CMover::Update()
{
    const float deltaTime = GetFrameTime();

    m_velocity += m_acceleration * deltaTime;
    m_position += m_velocity * deltaTime;
    m_acceleration = Vector2Zero();

    // Update rotation based on velocity direction if moving
    if (Vector2Length(m_velocity) > 0.01f) {
        m_rotation = atan2f(m_velocity.y, m_velocity.x);
    }
}

void CMover::Draw() const
{
    // Triangle parameters
    constexpr float length = 20.0f;     // From center to tip
    constexpr float baseBack = 12.0f;   // How far the base center is behind the center
    constexpr float halfWidth = 8.0f;   // Half width of the base

    // Forward and right vectors from rotation
    const Vector2 forward = {cosf(m_rotation), sinf(m_rotation)};
    const Vector2 right = {-sinf(m_rotation), cosf(m_rotation)};

    // Compute vertices
    const Vector2 center = m_position;
    const Vector2 tip = center + forward * length;
    const Vector2 baseCenter = center - forward * baseBack;
    const Vector2 left = baseCenter - right * halfWidth;
    const Vector2 rightPt = baseCenter + right * halfWidth;

    // Draw oriented triangle representing direction
    DrawTriangleLines(tip, left, rightPt, RED);
}

const Vector2& CMover::GetPosition() const
{
    return m_position;
}

const Vector2& CMover::GetVelocity() const
{
    return m_velocity;
}

const Vector2& CMover::GetAcceleration() const
{
    return m_acceleration;
}

float CMover::GetRotation() const
{
    return m_rotation;
}

void CMover::SetPosition(const Vector2& position)
{
    m_position = position;
}

void CMover::SetVelocity(const Vector2& velocity)
{
    m_velocity = velocity;
}

void CMover::SetAcceleration(const Vector2& acceleration)
{
    m_acceleration = acceleration;
}

void CMover::SetRotation(float rotation)
{
    m_rotation = std::fmod(rotation, 2.0f * PI);
}
void CMover::ApplyForce(const Vector2& force)
{
    m_acceleration += force;
}
