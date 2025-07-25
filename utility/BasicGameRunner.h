#pragma once

#ifndef INCLUDE_BASIC_GAME_RUNNER_H
#define INCLUDE_BASIC_GAME_RUNNER_H

#include <raylib.h>
#include <cstdint>
#include <functional>
#include <string>

class CBasicGameRunner
{
public:
    CBasicGameRunner();
    CBasicGameRunner(const int32_t width, const int32_t height);
    CBasicGameRunner(const int32_t width, const int32_t height, const int32_t targetFPS);
    ~CBasicGameRunner() = default;

    CBasicGameRunner(const CBasicGameRunner&) = delete;
    CBasicGameRunner& operator=(const CBasicGameRunner&) = delete;
    CBasicGameRunner(CBasicGameRunner&&) = delete;
    CBasicGameRunner& operator=(CBasicGameRunner&&) = delete;

public:
    void RunGame();
    void StopGame();

    CBasicGameRunner& SetInitCallback(std::function<void()>&& initCallback);
    CBasicGameRunner& SetDeInitCallback(std::function<void()>&& deInitCallback);
    CBasicGameRunner& SetUpdateCallback(std::function<void()>&& updateCallback);
    CBasicGameRunner& SetDrawWorldCallback(std::function<void()>&& drawWorldCallback);
    CBasicGameRunner& SetDrawUiCallback(std::function<void()>&& drawUiCallback);

    CBasicGameRunner& SetGameTitle(const std::string& title);
    CBasicGameRunner& SetGameFPS(const int32_t targetFPS);
    CBasicGameRunner& SetDrawCameraInfo(const bool isDrawCameraInfo);
    CBasicGameRunner& SetDrawFPS(const bool isDrawFPS);
    CBasicGameRunner& SetBackgroundColor(const Color& color);

    CBasicGameRunner& AddKeyboardControlsInfo(std::string&& info);
    CBasicGameRunner& SetInfoFontColor(const Color& color);

    CBasicGameRunner& ResetCamera();

    [[nodiscard]] int32_t GetWindowWidth() const;
    [[nodiscard]] int32_t GetWindowHeight() const;
    [[nodiscard]] Vector2 GetWindowCenterPosition() const;
    [[nodiscard]] Vector2 GetMouseWorldPosition() const;

private:
    std::string m_windowTitle = "Raylib - Nature of Code";
    const int32_t m_windowWidth = 800;
    const int32_t m_windowHeight = 600;
    int32_t m_windowTargetFPS = 60;

    Color m_backgroundColor = RAYWHITE;
    Color m_infoFontColor = BLACK;

    std::vector<std::string> m_keyboardControlsInfo;

    Camera2D m_camera{};

    bool m_isGameRunning = true;
    bool m_isDrawCameraInfo = false;
    bool m_isDrawFPS = true;

    std::function<void()> m_initCallback;
    std::function<void()> m_deInitCallback;
    std::function<void()> m_updateCallback;
    std::function<void()> m_drawWorldCallback;
    std::function<void()> m_drawUiCallback;
};

#endif // INCLUDE_BASIC_GAME_RUNNER_H
