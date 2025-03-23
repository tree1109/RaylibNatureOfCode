#include <raylib.h>
#include <raymath.h>
#include <array>
#include <numbers>
#include <cstdint>
#include <cmath>
#include <deque>
#include <functional>
#include <vector>

#include "utility/Math.h"
#include "utility/Tool.h"

#include "utility/PathFinder.h"

class Pendulum
{
public:
    Pendulum(const Vector2& pivotPos, const float r)
        : PivotPos(pivotPos)
        , R(r)
    {
    }

    void Update()
    {
        const float deltaTime = GetFrameTime();

        AngularVelocity += AngularAcceleration * deltaTime;
        Angle += AngularVelocity * deltaTime;

        AngularAcceleration = 0.0f;
    }

    void Draw() const
    {
        const Vector2 bobPos = PivotPos + Vector2Rotate(Vector2UnitX, Angle) * R;
        DrawLineV(PivotPos, bobPos, BLACK);
        DrawCircleLinesV(bobPos, 10.0f, BLACK);
    }

    void ApplyForce(const Vector2 force)
    {
        const float forceAngle = Vector2Angle(Vector2UnitX, force);
        const float angle = forceAngle - Angle;
        const float forceMag = Vector2Length(force);
        const float torque = R * forceMag * sinf(angle);
        const float angularAcceleration = torque / R;
        AngularAcceleration += angularAcceleration;
    }

    Vector2 PivotPos = Vector2Zeros;
    float R = 1.0f;
    float Angle = 0.0f;
    float AngularVelocity = 0.0f;
    float AngularAcceleration = 0.0f;
};

class DoublePendulum
{
public:
    DoublePendulum(const Vector2& pivotPos, const float r, const Vector2& startAngle)
        : PivotPos(pivotPos)
        , R1(r)
        , R2(r)
        , Angle1(startAngle.x)
        , Angle2(startAngle.y)
    {
    }

    void Update()
    {
        const float deltaTime = GetFrameTime();

        constexpr float G = 1000.0f;

        // Cute flower pattern.
        if (false) {
            AngularAcceleration1 = 0.1f;
            AngularAcceleration2 = 0.22f;
        }
        else
        {
            // https://scienceworld.wolfram.com/physics/DoublePendulum.html
            const float angle1 = Angle1 - PI * 0.5f;
            const float angle2 = Angle2 - PI * 0.5f;

            const float num1 = -G * (2.0f * Mass1 + Mass2) * sinf(angle1);
            const float num2 = -Mass2 * G * sinf(angle1 - 2.0f * angle2);
            const float num3 = -2.0f * sinf(angle1 - angle2) * Mass2;
            const float num4 = AngularVelocity2 * AngularVelocity2 * R2 + AngularVelocity1 * AngularVelocity1 * R1 * cosf(angle1 - angle2);
            const float den = R1 * (2.0f * Mass1 + Mass2 - Mass2 * cosf(2.0f * angle1 - 2.0f * angle2));
            AngularAcceleration1 = (num1 + num2 + num3 * num4) / den;
            const float num5 = 2.0f * sinf(angle1 - angle2);
            const float num6 = (AngularVelocity1 * AngularVelocity1 * R1 * (Mass1 + Mass2));
            const float num7 = G * (Mass1 + Mass2) * cosf(angle1);
            const float num8 = AngularVelocity2 * AngularVelocity2 * R2 * Mass2 * cosf(angle1 - angle2);
            const float den2 = R2 * (2.0f * Mass1 + Mass2 - Mass2 * cosf(2.0f * angle1 - 2.0f * angle2));
            AngularAcceleration2 = (num5 * (num6 + num7 + num8)) / den2;
        }

        AngularVelocity1 += AngularAcceleration1 * deltaTime;
        AngularVelocity2 += AngularAcceleration2 * deltaTime;
        Angle1 += AngularVelocity1 * deltaTime;
        Angle2 += AngularVelocity2 * deltaTime;

        AngularAcceleration1 = 0.0f;
        AngularAcceleration2 = 0.0f;
    }

    void Draw() const
    {
        const Vector2 bobPos1 = PivotPos + Vector2Rotate(Vector2UnitX, Angle1) * R1;
        const Vector2 bobPos2 = bobPos1 + Vector2Rotate(Vector2UnitX, Angle2) * R2;
        DrawLineV(PivotPos, bobPos1, GREEN);
        DrawLineV(bobPos1, bobPos2, LIME);
        DrawCircleLinesV(bobPos1, 10.0f, BLACK);
        DrawCircleLinesV(bobPos2, 10.0f, BLACK);
    }

    Vector2 GetBob1Pos() const
    {
        return PivotPos + Vector2Rotate(Vector2UnitX, Angle1) * R1;
    }

    Vector2 GetBob2Pos() const
    {
        return GetBob1Pos() + Vector2Rotate(Vector2UnitX, Angle2) * R2;
    }

    Vector2 PivotPos = Vector2Zeros;
    float R1 = 1.0f;
    float R2 = 1.0f;
    float Angle1 = 0.0f;
    float Angle2 = 0.0f;
    float AngularVelocity1 = 0.0f;
    float AngularVelocity2 = 0.0f;
    float AngularAcceleration1 = 0.0f;
    float AngularAcceleration2 = 0.0f;
    float Mass1 = 10.0f;
    float Mass2 = 10.0f;
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

    constexpr Vector2 center = { windowWidth * 0.5f , windowHeight * 0.5f };

    Pendulum pendulum = { center, 200.0f };

    DoublePendulum doublePendulum = { center, 100.0f, { -PI * 0.25f, -PI * 0.125f } };
    std::deque<Vector2> trace;

    while (!WindowShouldClose())
    {
        const float time = static_cast<float>(GetTime());
        const float deltaTime = GetFrameTime();

        // Update.
        {
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
            {
                const float angle = Vector2Angle(Vector2UnitX, GetMousePosition() - pendulum.PivotPos);
                pendulum.Angle = angle;
                pendulum.AngularVelocity = 0.0f;
            }
            pendulum.AngularVelocity *= 0.99f;
            pendulum.ApplyForce(Vector2{ 0.0f, 10.0f });
            pendulum.Update();

            doublePendulum.Update();
            trace.emplace_back(doublePendulum.GetBob2Pos());
            while (trace.size() > 10000)
            {
                trace.pop_front();
            }
        }

        BeginTextureMode(canvas);
        ClearBackground(RAYWHITE);
        // Draw.
        {
            pendulum.Draw();

            doublePendulum.Draw();
            for (int32_t i = 0; i < trace.size() - 1; ++i)
            {
                DrawLineV(trace[i], trace[i + 1], PINK);
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