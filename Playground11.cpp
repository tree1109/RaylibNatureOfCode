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

int32_t main()
{
    // Window initialization.
    constexpr int32_t windowWidth = 800;
    constexpr int32_t windowHeight = 600;
    constexpr const char* pWindowTitle = "Raylib - Nature of Code";

    InitWindow(windowWidth, windowHeight, pWindowTitle);
    SetTargetFPS(60);

    auto canvas = LoadRenderTexture(windowWidth, windowHeight);

    constexpr Vector2 ScreenCenterPos = { windowWidth * 0.5f , windowHeight * 0.5f };

    Camera2D camera;
    camera.offset = Vector2Zeros;
    camera.target = Vector2Zeros;
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    while (!WindowShouldClose())
    {
        const float time = static_cast<float>(GetTime());
        const float deltaTime = GetFrameTime();

        // Update.
        {
            // Update camera.
            {
                // Translate based on mouse right click
                if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
                {
                    Vector2 delta = GetMouseDelta();
                    delta = Vector2Scale(delta, -1.0f / camera.zoom);
                    camera.target = Vector2Add(camera.target, delta);
                }

                // Zoom based on mouse wheel
                float wheel = GetMouseWheelMove();
                if (wheel != 0)
                {
                    // Get the world point that is under the mouse
                    const Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);

                    // Set the offset to where the mouse is
                    camera.offset = GetMousePosition();

                    // Set the target to match, so that the camera maps the world space point 
                    // under the cursor to the screen space point under the cursor at any zoom
                    camera.target = mouseWorldPos;

                    // Zoom increment
                    // Uses log scaling to provide consistent zoom speed
                    const float scale = 0.2f * wheel;
                    camera.zoom = Clamp(expf(logf(camera.zoom) + scale), 0.125f, 64.0f);
                }
            }
        }

        BeginTextureMode(canvas);
        ClearBackground(RAYWHITE);
        BeginMode2D(camera);
        // Draw.
        {
            tool::DrawReferenceCoordinate(ScreenCenterPos, time);
        }
        EndMode2D();
        EndTextureMode();

        BeginDrawing();
        ClearBackground(RAYWHITE);
        constexpr auto sourceRect = Rectangle{ 0.0f, 0.0f, windowWidth, -windowHeight };
        constexpr auto destRect = Rectangle{ 0.0f, 0.0f, windowWidth, windowHeight };
        DrawTexturePro(canvas.texture, sourceRect, destRect, { 0.0f, 0.0f }, 0.0f, WHITE);
        // Draw UI.
        {
            // Camera info.
            {
                DrawText(TextFormat("Camera Offset: (%.2f, %.2f)", camera.offset.x, camera.offset.y), 10, 100, 20, BLACK);
                DrawText(TextFormat("Camera Target: (%.2f, %.2f)", camera.target.x, camera.target.y), 10, 120, 20, BLACK);
                DrawText(TextFormat("Camera Zoom: %.2f", camera.zoom), 10, 140, 20, BLACK);
            }
        }
        DrawFPS(0, 0);
        EndDrawing();
    }
    CloseWindow();

    return 0;
}