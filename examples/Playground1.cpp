#include <raylib.h>
#include <format>

#include "utility/Math.h"

namespace
{
    class Walker
    {
    public:
        Walker(Vector2 position)
            : m_position(position)
        {
        }

        void Update()
        {
            m_prevPosition = m_position;

            const float maxStep = 2.0f;
            const float stepX = math::MapValue(math::PeringNoise(GetTime()), 0.0f, 1.0f, -maxStep, maxStep);
            const float stepY = math::MapValue(math::PeringNoise(GetTime() + 1233.0f), 0.0f, 1.0f, -maxStep, maxStep);
            m_position.x += stepX;
            m_position.y += stepY;
        }

        void Draw() const
        {
            DrawLineV(m_prevPosition, m_position, BLACK);
        }

        void DrawUI() const
        {
        }

    private:
        Vector2 m_position;
        Vector2 m_prevPosition;
    };
}

int32_t main()
{
    // Window initialization.
    constexpr int32_t windowWidth = 800;
    constexpr int32_t windowHeight = 450;
    constexpr const char* pWindowTitle = "Raylib - Nature of Code";

    InitWindow(windowWidth, windowHeight, pWindowTitle);
    SetTargetFPS(60);

    auto canvas = LoadRenderTexture(windowWidth, windowHeight);

    // Init.
    constexpr Vector2 center = { windowWidth * 0.5f , windowHeight * 0.5f };
    Walker walker(center);

    while (!WindowShouldClose())
    {
        // Update.
        {
            walker.Update();
        }

        BeginTextureMode(canvas);
        // Draw.
        {
            walker.Draw();
        }
        EndTextureMode();

        BeginDrawing();
        ClearBackground(RAYWHITE);
        // Draw UI.
        {
            walker.DrawUI();
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
