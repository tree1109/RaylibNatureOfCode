#include <raylib.h>
#include <raymath.h>
#include <array>
#include <numbers>
#include <cstdint>
#include <cmath>
#include <vector>

#include "utility/Math.h"

#include "utility/PathFinder.h"

namespace
{
    class SpaceShip
    {
    public:
        SpaceShip() = default;

        SpaceShip(Vector2 position)
            : m_position(position)
        {
        }

        void Update()
        {
            m_isThrusting = Vector2LengthSqr(m_acceleration) > 0.0f;

            // Limit the velocity.
            if (Vector2Length(m_velocity) >= 500.0f)
            {
                m_velocity = Vector2Normalize(m_velocity) * 500.0f;
            }

            m_velocity += m_acceleration * GetFrameTime();
            m_position += m_velocity * GetFrameTime();

            m_angularVelocity += m_angularAcceleration * GetFrameTime();
            m_angle += m_angularVelocity * GetFrameTime();

            m_acceleration = Vector2Zeros;
            m_angularAcceleration = 0.0f;
        }

        void Draw() const
        {
            // Ship body.
            constexpr float WIDTH = 50.0f;
            constexpr float HEIGHT = 60.0f;
            Vector2 front = Vector2{ HEIGHT * 0.5f, 0.0f };
            Vector2 back = Vector2{ -HEIGHT * 0.5f, -WIDTH * 0.5f };
            Vector2 back2 = Vector2{ -HEIGHT * 0.5f, WIDTH * 0.5f };
            front = Vector2Rotate(front, m_angle);
            back = Vector2Rotate(back, m_angle);
            back2 = Vector2Rotate(back2, m_angle);
            front += m_position;
            back += m_position;
            back2 += m_position;
            DrawTriangleLines(front, back, back2, BLACK);

            // Thrust.
            if (m_isThrusting)
            {
                constexpr float RADIUS = 20.0f;
                Vector2 thrust = Vector2{ -HEIGHT * 0.5f, 0.0f };
                thrust = Vector2Rotate(thrust, m_angle);
                thrust += m_position;
                DrawCircleLinesV(thrust, RADIUS, ORANGE);
            }
        }

        void ApplyForce(const Vector2& force)
        {
            m_acceleration += force;
        }

        void ApplyTorque(float torque)
        {
            m_angularAcceleration += torque;
        }

        Vector2 m_position = Vector2Zeros;
        Vector2 m_velocity = Vector2Zeros;
        Vector2 m_acceleration = Vector2Zeros;
        float m_angle = 0.0f;
        float m_angularVelocity = 0.0f;
        float m_angularAcceleration = 0.0f;

        bool m_isThrusting = false;
    };
}

int32_t main()
{
    // Window initialization.
    constexpr int32_t windowWidth = 800;
    constexpr int32_t windowHeight = 600;
    constexpr const char* pWindowTitle = "Raylib - Nature of Code";

    InitWindow(windowWidth, windowHeight, pWindowTitle);
    SetTargetFPS(60);

    auto canvas = LoadRenderTexture(windowWidth, windowHeight);

    Vector2 center = { windowWidth * 0.5f , windowHeight * 0.5f };

    SpaceShip mover{ center };

    Vector2 ballPosition = center;

    while (!WindowShouldClose())
    {
        const float time = GetTime();
        const float deltaTime = GetFrameTime();

        // Update.
        {
            const Vector2 mouseDirection = GetMousePosition() - mover.m_position;
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
            {
                mover.ApplyForce(Vector2Normalize(mouseDirection) * 100.0f);
            }
            mover.m_angle = Vector2Angle(Vector2UnitX, mouseDirection);

            mover.Update();

            // Ball.
            constexpr float AMPLITUDE = 100.0f;
            constexpr float PERIOD = 3.0f;
            Vector2 newPos = { AMPLITUDE * std::cos(2.0f * PI * time / PERIOD), 0.0f };
            newPos += center;
            ballPosition = newPos;
        }

        BeginTextureMode(canvas);
        ClearBackground(RAYWHITE);
        // Draw.
        {
            mover.Draw();

            // Ball.
            DrawCircleLinesV(ballPosition, 10.0f, RED);
        }
        EndTextureMode();

        BeginDrawing();
        ClearBackground(RAYWHITE);
        // Draw UI.
        {

        }
        constexpr auto sourceRect = Rectangle{ 0.0f, 0.0f, windowWidth, -windowHeight };
        constexpr auto destRect = Rectangle{ 0.0f, 0.0f, windowWidth, windowHeight };
        DrawTexturePro(canvas.texture, sourceRect, destRect, { 0.0f, 0.0f }, 0.0f, WHITE);
        DrawFPS(0, 0);
        EndDrawing();
    }
    CloseWindow();

    return 0;
}
