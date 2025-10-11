//
// Created by 莊瞻 on 2025/10/11.
//

#include "Mover.h"

#include <algorithm>

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
    // Triangle size
    constexpr float length = 30.0f; // Distance from center to tip
    constexpr float width = 20.0f; // Width of the base

    // Calculate the tip of the triangle (front)
    const Vector2 tip = {m_position.x + length * cosf(m_rotation), m_position.y + length * sinf(m_rotation)};

    // Calculate the two base points (back left and back right)
    const float baseAngle = m_rotation + PI; // Opposite direction
    constexpr float halfWidth = width * 0.5f;
    const Vector2 left = {m_position.x + halfWidth * cosf(baseAngle + PI * 0.5f),
                          m_position.y + halfWidth * sinf(baseAngle + PI * 0.5f)};
    const Vector2 right = {m_position.x + halfWidth * cosf(baseAngle - PI * 0.5f),
                           m_position.y + halfWidth * sinf(baseAngle - PI * 0.5f)};

    // Draw the triangle
    DrawTriangleLines(tip, left, right, RED);
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
