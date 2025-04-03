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

    void SetUpdateCallback(std::function<void()>&& updateCallback);
    void SetDrawWorldCallback(std::function<void()>&& drawWorldCallback);
    void SetDrawUiCallback(std::function<void()>&& drawUiCallback);

    void SetGameTitle(const std::string& title);
    void SetGameFPS(const int32_t targetFPS);
    void SetDrawCameraInfo(const bool isDrawCameraInfo);
    void SetDrawFPS(const bool isDrawFPS);

    void AddKeyboardControlsInfo(std::string&& info);

    void ResetCamera();

    [[nodiscard]] Vector2 GetWindowCenterPosition() const;

private:
    std::string m_windowTitle = "Raylib - Nature of Code";
    const int32_t m_windowWidth = 800;
    const int32_t m_windowHeight = 600;
    int32_t m_windowTargetFPS = 60;

    std::vector<std::string> m_keyboardControlsInfo;

    Camera2D m_camera{};

    bool m_isGameRunning = true;
    bool m_isDrawCameraInfo = false;
    bool m_isDrawFPS = true;

    std::function<void()> m_updateCallback;
    std::function<void()> m_drawWorldCallback;
    std::function<void()> m_drawUiCallback;
};

#endif // INCLUDE_BASIC_GAME_RUNNER_H
