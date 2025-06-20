#include "BasicGameRunner.h"

#include <raymath.h>
#include <algorithm>
#include <stdexcept>

CBasicGameRunner::CBasicGameRunner()
    : CBasicGameRunner(800, 600, 60)
{
}

CBasicGameRunner::CBasicGameRunner(const int32_t width, const int32_t height)
    : CBasicGameRunner(width, height, 60)
{
}

CBasicGameRunner::CBasicGameRunner(const int32_t width, const int32_t height, const int32_t targetFPS)
    : m_windowWidth(width)
    , m_windowHeight(height)
    , m_windowTargetFPS(targetFPS)
{
    AddKeyboardControlsInfo("Keyboard Controls");
    AddKeyboardControlsInfo("R - Reset Camera");
}

void CBasicGameRunner::RunGame()
{
    // Window initialization.
    InitWindow(m_windowWidth, m_windowHeight, m_windowTitle.c_str());
    SetTargetFPS(m_windowTargetFPS);

    // Texture data.
    auto canvas = LoadRenderTexture(m_windowWidth, m_windowHeight);
    const Rectangle sourceRectangle{
        0.0f,
        0.0f,
        static_cast<float>(m_windowWidth),
        -static_cast<float>(m_windowHeight)
    };
    const Rectangle destRectangle{
        0.0f,
        0.0f,
        static_cast<float>(m_windowWidth),
        static_cast<float>(m_windowHeight)
    };

    ResetCamera();

    // Initialize.
    if (m_initCallback) {
        m_initCallback();
    }

    // Update loop.
    while (!WindowShouldClose() && m_isGameRunning) {
        // Update.
        {
            // Update camera.
            {
                // Translate based on mouse right click
                if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
                    Vector2 delta = GetMouseDelta();
                    delta = Vector2Scale(delta, -1.0f / m_camera.zoom);
                    m_camera.target = Vector2Add(m_camera.target, delta);
                }

                // Zoom based on mouse wheel
                float wheel = GetMouseWheelMove();
                if (!FloatEquals(wheel, 0.0f)) {
                    // Get the world point that is under the mouse
                    const Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), m_camera);

                    // Set the offset to where the mouse is
                    m_camera.offset = GetMousePosition();

                    // Set the target to match, so that the camera maps the world space point 
                    // under the cursor to the screen space point under the cursor at any zoom
                    m_camera.target = mouseWorldPos;

                    // Zoom increment
                    // Uses log scaling to provide consistent zoom speed
                    const float scale = 0.2f * wheel;
                    m_camera.zoom = Clamp(expf(logf(m_camera.zoom) + scale), 0.125f, 64.0f);
                }

                // Reset camera if 'R' key is pressed
                if (IsKeyPressed(KEY_R)) {
                    ResetCamera();
                }
            }

            // Update world.
            if (m_updateCallback) {
                m_updateCallback();
            }
        }

        // Draw to canvas.
        BeginTextureMode(canvas);
        ClearBackground(m_backgroundColor);
        BeginMode2D(m_camera);
        // Draw world.
        if (m_drawWorldCallback) {
            m_drawWorldCallback();
        }
        EndMode2D();
        EndTextureMode();

        // Draw to screen.
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawTexturePro(canvas.texture, sourceRectangle, destRectangle, {0.0f, 0.0f}, 0.0f, WHITE);
        // Draw UI.
        {
            // Draw UI.
            if (m_drawUiCallback) {
                m_drawUiCallback();
            }

            // Keyboard info.
            for (int32_t i = 0; const std::string& info : m_keyboardControlsInfo) {
                constexpr int32_t fontSize = 20;
                const Color fontColor = m_infoFontColor;

                constexpr int32_t textXAlignment = 10;
                constexpr int32_t textYStartPos = 40;
                constexpr int32_t textYSpacing = 20;

                const int32_t textYPos = textYStartPos + i * textYSpacing;
                DrawText(info.c_str(), textXAlignment, textYPos, fontSize, fontColor);
                ++i;
            }

            // Camera info.
            if (m_isDrawCameraInfo) {
                DrawText(TextFormat("Camera Offset: (%.2f, %.2f)", m_camera.offset.x, m_camera.offset.y), 10, 100, 20,
                         BLACK);
                DrawText(TextFormat("Camera Target: (%.2f, %.2f)", m_camera.target.x, m_camera.target.y), 10, 120, 20,
                         BLACK);
                DrawText(TextFormat("Camera Zoom: %.2f", m_camera.zoom), 10, 140, 20, BLACK);
            }

            // Draw FPS.
            if (m_isDrawFPS) {
                DrawFPS(0, 0);
            }
        }
        EndDrawing();
    }

    // De-Initialize.
    if (m_deInitCallback) {
        m_deInitCallback();
    }

    CloseWindow();
}

void CBasicGameRunner::StopGame()
{
    m_isGameRunning = false;
}

CBasicGameRunner& CBasicGameRunner::SetInitCallback(std::function<void()>&& initCallback)
{
    m_initCallback = std::move(initCallback);
    return *this;
}

CBasicGameRunner& CBasicGameRunner::SetDeInitCallback(std::function<void()>&& deInitCallback)
{
    m_deInitCallback = std::move(deInitCallback);
    return *this;
}

CBasicGameRunner& CBasicGameRunner::SetUpdateCallback(std::function<void()>&& updateCallback)
{
    m_updateCallback = std::move(updateCallback);
    return *this;
}

CBasicGameRunner& CBasicGameRunner::SetDrawWorldCallback(std::function<void()>&& drawWorldCallback)
{
    m_drawWorldCallback = std::move(drawWorldCallback);
    return *this;
}

CBasicGameRunner& CBasicGameRunner::SetDrawUiCallback(std::function<void()>&& drawUiCallback)
{
    m_drawUiCallback = std::move(drawUiCallback);
    return *this;
}

CBasicGameRunner& CBasicGameRunner::SetGameTitle(const std::string& title)
{
    m_windowTitle = title;
    return *this;
}

CBasicGameRunner& CBasicGameRunner::SetGameFPS(const int32_t targetFPS)
{
    constexpr int32_t minFPS = 1;
    constexpr int32_t maxFPS = 6000;
    m_windowTargetFPS = std::clamp(targetFPS, minFPS, maxFPS);
    return *this;
}

CBasicGameRunner& CBasicGameRunner::SetDrawCameraInfo(const bool isDrawCameraInfo)
{
    m_isDrawCameraInfo = isDrawCameraInfo;
    return *this;
}

CBasicGameRunner& CBasicGameRunner::SetDrawFPS(const bool isDrawFPS)
{
    m_isDrawFPS = isDrawFPS;
    return *this;
}

CBasicGameRunner & CBasicGameRunner::SetBackgroundColor(const Color &color)
{
    m_backgroundColor = color;
    return *this;
}

CBasicGameRunner& CBasicGameRunner::AddKeyboardControlsInfo(std::string&& info)
{
    m_keyboardControlsInfo.emplace_back(std::move(info));
    return *this;
}

CBasicGameRunner& CBasicGameRunner::SetInfoFontColor(const Color& color)
{
    m_infoFontColor = color;
    return *this;
}

CBasicGameRunner& CBasicGameRunner::ResetCamera()
{
    m_camera.offset = Vector2Zeros;
    m_camera.target = Vector2Zeros;
    m_camera.rotation = 0.0f;
    m_camera.zoom = 1.0f;
    return *this;
}

int32_t CBasicGameRunner::GetWindowWidth() const
{
    return m_windowWidth;
}

int32_t CBasicGameRunner::GetWindowHeight() const
{
    return m_windowHeight;
}

Vector2 CBasicGameRunner::GetWindowCenterPosition() const
{
    return Vector2{
        static_cast<float>(m_windowWidth) * 0.5f,
        static_cast<float>(m_windowHeight) * 0.5f
    };
}

Vector2 CBasicGameRunner::GetMouseWorldPosition() const
{
    const auto mousePos = GetMousePosition();
    const auto mouseWorldPos = GetScreenToWorld2D(mousePos, m_camera);
    return mouseWorldPos;
}
