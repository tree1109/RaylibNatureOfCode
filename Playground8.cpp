#include <raylib.h>
#include <raymath.h>
#include <array>
#include <numbers>
#include <cstdint>
#include <cmath>
#include <vector>

#include "utility/Math.h"

#include "utility/PathFinder.h"

class Oscillator
{
public:
    Oscillator(Vector2 originPosition)
        : m_originPosition(originPosition)
    {
    }

    void SetAngle(const Vector2& angle)
    {
        m_angle = angle;
    }

    void SetRandomAngle()
    {
        m_angle = Vector2{ math::GetRandomValue(0.0f, 2.0f * PI), math::GetRandomValue(0.0f, 2.0f * PI) };
    }

    void SetAmplitude(const Vector2& amplitude)
    {
        m_amplitude = amplitude;
    }

    void SetAngularVelocity(const Vector2& angularVelocity)
    {
        m_angularVelocity = angularVelocity;
    }

    void SetColor(const Color& circleColor, const Color& lineColor)
    {
        m_circleColor = circleColor;
        m_lineColor = lineColor;
    }

    void Update()
    {
        m_angle += m_angularVelocity * GetFrameTime();
        m_position = m_originPosition + m_amplitude * Vector2{ std::sin(m_angle.x), std::sin(m_angle.y) };
    }

    void Draw() const
    {
        DrawLineV(m_originPosition, m_position, m_lineColor);
        DrawCircleLinesV(m_position, 10.0f, m_circleColor);
    }

    Vector2 m_originPosition;
    Vector2 m_position = { 0.0f, 0.0f };
    Vector2 m_amplitude = { 100.0f, 100.0f };
    Vector2 m_angle = { 0.0f, 0.0f };
    Vector2 m_angularVelocity = { 0.0f, 0.0f };

    Color m_circleColor = BLACK;
    Color m_lineColor = BLACK;
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

    const Vector2 center = { windowWidth * 0.5f , windowHeight * 0.5f };

    std::vector<Oscillator> oscillators;
    // Random.
    for (int32_t i = 0; i < 10; ++i)
    {
        oscillators.emplace_back(center);

        oscillators.back().SetAngularVelocity({math::GetRandomValue(-1.0f, 1.0f), math::GetRandomValue(-1.0f, 1.0f) });
        oscillators.back().SetAmplitude({ math::GetRandomValue(10.0f, center.x), math::GetRandomValue(10.0f, center.y) });
        oscillators.back().SetRandomAngle();
    }
    // Wave.
    for (int32_t i = 0; i < windowWidth; i += 10)
    {
        oscillators.emplace_back(Vector2{ static_cast<float>(i), center.y });

        oscillators.back().SetAngularVelocity({ 2.0f,  5.0f });
        oscillators.back().SetAmplitude({ 33.0f, static_cast<float>(windowWidth - i) * 0.1f});
        oscillators.back().SetColor(BLUE, LIGHTGRAY);
        oscillators.back().SetAngle({ i * 0.01f + PI * 0.5f, i * 0.01f });
    }

    // Another wave.
    float startAngle = 0.0f;
    float startAngle2 = 35.0f;
    constexpr float angleDelta = (2.0f * PI / windowWidth) * 5.0f;
    constexpr float angularVelocity = angleDelta;
    constexpr float angleDelta2 = (2.0f * PI / windowWidth);
    constexpr float angularVelocity2 = angleDelta2 * 0.333f;
    std::vector<Vector2> wavePoints;

    while (!WindowShouldClose())
    {
        const float time = static_cast<float>(GetTime());
        const float deltaTime = GetFrameTime();

        // Update.
        {
            for (auto& oscillator : oscillators)
            {
                oscillator.Update();
            }

            // Another wave.
            {
                wavePoints.clear();

                // wave1.
                std::vector<float> waveValues;
                float angle = startAngle;
                for (int32_t x = 0; x < windowWidth; ++x)
                {
                    const float value = std::sin(angle) + std::sin(angle * 2.0f) * 0.5f + std::sin(angle * 5.0f) * 0.2f;
                    waveValues.emplace_back(value);

                    angle += angleDelta;
                }
                startAngle += angularVelocity;

                // wave2.
                std::vector<float> waveValues2;
                float angle2 = startAngle2;
                for (int32_t x = 0; x < windowWidth; ++x)
                {
                    const float value = std::sin(angle2 * 1.14514f);
                    waveValues2.emplace_back(value);

                    angle2 += angleDelta;
                }
                startAngle2 += angularVelocity2;

                // combine wave.
                for (int32_t x = 0; x < windowWidth; ++x)
                {
                    const float y = center.y * 1.5f + (waveValues[x] * 20.0f + waveValues2[x] * 15.0f);
                    Vector2 point = { static_cast<float>(x), y };
                    wavePoints.emplace_back(point);
                }
            }
        }

        BeginTextureMode(canvas);
        ClearBackground(RAYWHITE);
        // Draw.
        {
            for (const auto& oscillator : oscillators)
            {
                oscillator.Draw();
            }

            // Another wave.
            DrawLineStrip(wavePoints.data(), static_cast<int>(wavePoints.size()), SKYBLUE);
            for (const auto& point : wavePoints)
            {
                if (std::fmod(point.x, 10.0f) != 0.0f) continue;
                DrawLineV(point + Vector2{0.0f, 10.0f}, { point.x, static_cast<float>(windowHeight) }, BLUE);
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