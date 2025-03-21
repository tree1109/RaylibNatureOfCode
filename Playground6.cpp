#include <raylib.h>
#include <raymath.h>
#include <array>
#include <numbers>
#include <cstdint>
#include <cmath>
#include <vector>

#include "utility/Math.h"

#include "utility/PathFinder.h"

static float GetRadius(float mass)
{
    // Volume as mass.
    return powf(mass / PI, 1.0f / 3.0f) * 2.0f;
}

class Body
{
public:
    Body() = default;

    Body(Vector2 position, float mass)
        : m_position(position)
        , m_mass(mass)
    {
    }

    void Update()
    {
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
        const float radius = GetRadius(m_mass);
        DrawCircleLinesV(m_position, radius, BLACK);

        Vector2 end = Vector2{ radius, 0.0f };
        end = Vector2Rotate(end, m_angle);
        end += m_position;
        DrawLineV(m_position, end, BLACK);
    }

    void ApplyForce(const Vector2& force)
    {
        m_acceleration += force / m_mass;
    }

    Vector2 GetAttractForce(const Body& mover) const
    {
        constexpr float G = 9.8f;

        // Calculate the force of attraction.
        Vector2 force = m_position - mover.m_position;
        const float distance = Clamp(Vector2Length(force), 4.0f, 4096.0f);
        force = Vector2Normalize(force);

        // Calculate the strength of the force.
        const float strength = G * m_mass * mover.m_mass / (distance * distance);
        force *= strength;

        return force;
    }

    float m_mass = 1.0f;
    Vector2 m_position = Vector2Zeros;
    Vector2 m_velocity = Vector2Zeros;
    Vector2 m_acceleration = Vector2Zeros;
    float m_angle = 0.0f;
    float m_angularVelocity = 0.0f;
    float m_angularAcceleration = 0.0f;
};

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

    std::vector<Body> movers;
    for (int32_t i = 0; i < 100; ++i)
    {
        Vector2 pos = Vector2(GetRandomValue(0, windowWidth), GetRandomValue(0, windowHeight));
        movers.emplace_back(pos, GetRandomValue(10, 100));
    }
    while (!WindowShouldClose())
    {
        const float time = GetTime();
        const float deltaTime = GetFrameTime();

        // Update.
        {
            for (auto& mover : movers)
            {
                for (auto& otherMover : movers)
                {
                    if (&mover != &otherMover)
                    {
                        Vector2 force = otherMover.GetAttractForce(mover);
                        mover.ApplyForce(force);
                    }
                }
            }

            for (auto& mover : movers)
            {
                mover.m_angularVelocity = mover.m_acceleration.x;

                mover.Update();
            }
        }

        BeginTextureMode(canvas);
        ClearBackground(RAYWHITE);
        // Draw.
        {
            for (auto& mover : movers)
            {
                mover.Draw();
            }
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