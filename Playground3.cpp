#include <raylib.h>
#include <raymath.h>
#include <array>
#include <numbers>
#include <cstdint>
#include <cmath>
#include <vector>

#include "utility/Math.h"

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
        WallBounce();
        //WallForce();

        const bool contactGround = m_position.y + GetRadius() >= GetScreenHeight() - 1.0f;
        if (contactGround)
        {
            const float c = 0.1f;
            const float normal = m_mass * 9.8f;
            const float frictionSize = c * normal;
            Vector2 friction = Vector2Normalize(m_velocity) * -1.0f * frictionSize;
            ApplyForce(friction);
        }


        // Limit the velocity.
        if (Vector2Length(m_velocity) >= 500.0f)
        {
            m_velocity = Vector2Normalize(m_velocity) * 500.0f;
        }

        m_velocity += m_acceleration * GetFrameTime();
        m_position += m_velocity * GetFrameTime();

        m_acceleration = Vector2Zeros;
    }

    void Draw() const
    {
        DrawCircleLinesV(m_position, GetRadius(), BLACK);
    }

    void ApplyForce(const Vector2& force)
    {
        m_acceleration += force / m_mass;
    }

    void WallBounce()
    {
        if (m_position.x + GetRadius() > GetScreenWidth())
        {
            m_position.x = GetScreenWidth() - GetRadius();
            m_velocity.x *= -0.9f;
        }
        else if (m_position.x - GetRadius() < 0.0f)
        {
            m_position.x = GetRadius();
            m_velocity.x *= -0.9f;
        }
        if (m_position.y + GetRadius() > GetScreenHeight())
        {
            m_position.y = GetScreenHeight() - GetRadius();
            m_velocity.y *= -0.9f;
        }
        else if (m_position.y - GetRadius() < 0.0f)
        {
            m_position.y = GetRadius();
            m_velocity.y *= -0.9f;
        }
    }

    void WallForce()
    {
        const float leftDistance = m_position.x - GetRadius();
        const float rightDistance = GetScreenWidth() - m_position.x - GetRadius();
        const float topDistance = m_position.y - GetRadius();
        const float bottomDistance = GetScreenHeight() - m_position.y - GetRadius();

        const float wallForceDistance = 64.0f;
        const float wallForce = 100.0f * m_mass;
        // wall force, more closer, more force.
        if (leftDistance < wallForceDistance)
        {
            float wallForceFactor = 1.0f - leftDistance / wallForceDistance;
            Vector2 force = Vector2(wallForce * wallForceFactor, 0.0f);
            ApplyForce(force);
        }
        if (rightDistance < wallForceDistance)
        {
            float wallForceFactor = 1.0f - rightDistance / wallForceDistance;
            Vector2 force = Vector2(-wallForce * wallForceFactor, 0.0f);
            ApplyForce(force);
        }
        if (topDistance < wallForceDistance)
        {
            float wallForceFactor = 1.0f - topDistance / wallForceDistance;
            Vector2 force = Vector2(0.0f, wallForce * wallForceFactor);
            ApplyForce(force);
        }
        if (bottomDistance < wallForceDistance)
        {
            float wallForceFactor = 1.0f - bottomDistance / wallForceDistance;
            Vector2 force = Vector2(0.0f, -wallForce * wallForceFactor);
            ApplyForce(force);
        }
    }

    float GetRadius() const
    {
        return m_mass * 4.0f;
    }

    float m_mass = 1.0f;
    Vector2 m_position = Vector2Zeros;
    Vector2 m_velocity = Vector2Zeros;
    Vector2 m_acceleration = Vector2Zeros;
};

class Liquid
{
public:
    Liquid(Vector2 position, Vector2 size, float c)
    {
        m_rect.x = position.x - size.x * 0.5f;
        m_rect.y = position.y - size.y * 0.5f;
        m_rect.width = size.x;
        m_rect.height = size.y;
        m_c = c;
    }

    bool IsContract(const Body& mover)
    {
        return CheckCollisionCircleRec(mover.m_position, mover.GetRadius(), m_rect);
    }

    Vector2 CalculateDragForce(const Body& mover)
    {
        const float speed = Vector2Length(mover.m_velocity);
        const float dragMagnitude = m_c * speed * speed;
        Vector2 dragForce = Vector2Negate(Vector2Normalize(mover.m_velocity)) * dragMagnitude;
        return dragForce;
    }

    void Draw() const
    {
        DrawRectangleLinesEx(m_rect, 1.0f, BLUE);
    }

    Rectangle m_rect = { 0.0f, 0.0f, 100.0f, 100.0f };
    float m_c = 0.1f;
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
        float mass = 1.0f + math::GetRandomValue() * 4.0f;
        movers.emplace_back(pos, mass);
    }

    //constexpr float gravityConst = 9.8f;
    constexpr float gravityConst = 98.0f;
    Vector2 gravity = { 0.0f, gravityConst };

    float c = 0.5f;
    //float c = 0.9f;
    Liquid liquid(center, {100.0f, 100.0f}, c);

    while (!WindowShouldClose())
    {
        const float time = GetTime();
        const float deltaTime = GetFrameTime();

        // Update.
        {
            for (auto& mover : movers)
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                {
                    Vector2 wind = Vector2(100.0f, 0.0f);
                    mover.ApplyForce(wind);
                }
                if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
                {
                    float distanceToMouse = Vector2Distance(mover.m_position, GetMousePosition());
                    if (distanceToMouse < 64.0f)
                    {
                        Vector2 toMouseForce = Vector2Normalize(GetMousePosition() - mover.m_position) * 1000.0f;
                        mover.ApplyForce(toMouseForce * mover.m_mass);
                    }
                }
                if (liquid.IsContract(mover))
                {
                    Vector2 dragForce = liquid.CalculateDragForce(mover);
                    mover.ApplyForce(dragForce);
                }
                mover.ApplyForce(gravity * mover.m_mass);
                mover.Update();
            }
        }

        BeginTextureMode(canvas);
        ClearBackground(RAYWHITE);
        // Draw.
        {
            if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
            {
                DrawCircleLinesV(GetMousePosition(), 64.0f, GREEN);
            }
            for (const auto& mover : movers)
            {
                mover.Draw();
            }
            liquid.Draw();

            DrawCircleLinesV(movers[0].m_position, 8.0f, RED);
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