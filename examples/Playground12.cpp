#include <raylib.h>
#include <raymath.h>

#include <algorithm>
#include <array>
#include <cmath>
#include <concepts>
#include <cstdint>
#include <deque>
#include <filesystem>
#include <format>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <numbers>
#include <numeric>
#include <random>
#include <set>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "game_object/Emitter.h"
#include "game_object/RandomField.h"
#include "utility/BasicGameRunner.h"
#include "utility/Math.h"
#include "utility/SafeImage.h"
#include "utility/TextureManager.h"
#include "utility/Tool.h"

// Template for the main function.
int32_t main()
{
    CBasicGameRunner game;
    game.SetBackgroundColor(BLACK)
        .SetInfoFontColor(RAYWHITE)
        .AddKeyboardControlsInfo("E - Clear Emitters")
        .AddKeyboardControlsInfo("Left Click - Blow wind according mouse position");

    CTextureManager textureManager;

    const auto center = game.GetWindowCenterPosition();

    std::vector<CEmitter> fireEmitters;
    CRandomField randomField{100.0f};

    auto reset = [&]() {
        fireEmitters.clear();
        fireEmitters.emplace_back(center);

        fireEmitters.back()
            .SetRandomColor(true)
            .SetTint(PINK)
            .SetDrawParticleWithTexture(true)
            .SetParticleTexture(textureManager.GetTexture("fire"))
            .SetParticleSpawnCountPerFrame(1);
    };

    // Init here.
    auto init = [&]() {
        const auto fireImage = tool::GenerateBlurCircleImage(WHITE, 0.1f);
        textureManager.LoadTexture("fire", fireImage);

        reset();
    };

    // DeInit here.
    auto deInit = [&]() { textureManager.UnloadTextures(); };

    // Update logic here
    auto update = [&] {
        if (IsKeyPressed(KEY_E)) {
            reset();
        }

        // Wind force.
        const Vector2 windForce = []() -> Vector2 {
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                const auto xPos = GetMousePosition().x;
                const auto windowWidth = GetScreenWidth();
                const float forceX = math::MapValue(xPos, 0.0f, static_cast<float>(windowWidth), -100.0f, 100.0f);
                return Vector2(forceX, 0.0f);
            }
            return {};
        }();

        const Vector2 floatForce = {0.0f, -50.0f};

        for (auto& emitter : fireEmitters) {
            emitter.ApplyForce(windForce);
            emitter.ApplyForce(floatForce);
            emitter.ApplyForceField(randomField);
            emitter.Update();
        }
    };

    // Draw world here
    auto drawWorld = [&] {
        const double time = GetTime();
        const float deltaTime = GetFrameTime();

        BeginBlendMode(BLEND_ADDITIVE);
        for (const auto& emitter : fireEmitters) {
            emitter.Draw();
        }
        EndBlendMode();
    };

    // Draw UI here
    auto drawUi = [&] {};

    // Set callbacks and run the game.
    game.SetInitCallback(init)
        .SetDeInitCallback(deInit)
        .SetUpdateCallback(update)
        .SetDrawWorldCallback(drawWorld)
        .SetDrawUiCallback(drawUi)
        .RunGame();

    return 0;
}
