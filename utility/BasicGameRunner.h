#pragma once

#ifndef INCLUDE_BASIC_GAME_RUNNER_H
#define INCLUDE_BASIC_GAME_RUNNER_H

#include <raylib.h>
#include <functional>
#include <string>

#include "IGame.h"

#include "service/IService.h"
#include "manager/IManager.h"

namespace playground
{
    class IPlayground;
}

class CBasicGameRunner final : public IGame
{
public:
    CBasicGameRunner();
    CBasicGameRunner(const int32_t width, const int32_t height);
    CBasicGameRunner(const int32_t width, const int32_t height, const int32_t targetFPS);
    ~CBasicGameRunner() override = default;

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

    CBasicGameRunner& ResetSettingsToDefault();
    CBasicGameRunner& SetGameTitle(const std::string& title);
    CBasicGameRunner& SetGameFPS(const int32_t targetFPS);
    CBasicGameRunner& SetDrawCameraInfo(const bool isDrawCameraInfo);
    CBasicGameRunner& SetDrawFPS(const bool isDrawFPS);
    CBasicGameRunner& SetBackgroundColor(const Color& color) override;
    CBasicGameRunner& SetClearBackgroundEachFrame(const bool isClear) override;
    CBasicGameRunner& SetCanMoveCameraByMouse(const bool isCanMove);

    CBasicGameRunner& AddKeyboardControlsInfo(std::string&& info) override;
    CBasicGameRunner& SetInfoFontColor(const Color& color) override;

    CBasicGameRunner& ResetCamera();

    CBasicGameRunner& AddPlayground(std::unique_ptr<playground::IPlayground>&& playground);
    CBasicGameRunner& NextPlayground();
    CBasicGameRunner& PreviousPlayground();

    CBasicGameRunner& AddService(std::unique_ptr<IService>&& service);
    CBasicGameRunner& AddManager(std::unique_ptr<IManager>&& manager);

    IService& GetService(const std::string_view& serviceName) override;
    IManager& GetManager(const std::string_view& managerName) override;

    [[nodiscard]] int32_t GetWindowWidth() const override;
    [[nodiscard]] int32_t GetWindowHeight() const override;
    [[nodiscard]] Vector2 GetWindowCenterPosition() const override;
    [[nodiscard]] Vector2 GetMouseWorldPosition() const override;
    [[nodiscard]] double GetTime() const override;
    [[nodiscard]] float GetFrameTime() const override;

private:
    std::string m_windowTitle = "Raylib - Nature of Code";
    const int32_t m_windowWidth = 800;
    const int32_t m_windowHeight = 600;
    int32_t m_windowTargetFPS = 60;

    Color m_backgroundColor = RAYWHITE;
    Color m_infoFontColor = BLACK;

    std::vector<std::string> m_keyboardControlsInfo;

    Camera2D m_camera{};

    double m_time = 0.0;
    float m_frameTime = 0.0f;

    bool m_isGameRunning = true;
    bool m_isDrawCameraInfo = false;
    bool m_isDrawFPS = true;
    bool m_isClearBackgroundEachFrame = true;
    bool m_isCanMoveCameraByMouse = false;
    bool m_flagClearBackgroundFirstFrame = true;

    std::function<void()> m_initCallback;
    std::function<void()> m_deInitCallback;
    std::function<void()> m_updateCallback;
    std::function<void()> m_drawWorldCallback;
    std::function<void()> m_drawUiCallback;

    std::vector<std::unique_ptr<playground::IPlayground>> m_playgrounds;

    std::unordered_map<std::string, std::unique_ptr<IService>> m_services;
    std::unordered_map<std::string, std::unique_ptr<IManager>> m_managers;
};

#endif // INCLUDE_BASIC_GAME_RUNNER_H
