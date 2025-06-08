#include <raylib.h>

#include <vector>
#include <cstdint>
#include <format>
#include <functional>
#include <memory>
#include <numeric>
#include <string>

#include "game_object/Attractor.h"
#include "game_object/Emitter.h"

#include "utility/BasicGameRunner.h"
#include "utility/Math.h"

#include "game_object/Repeller.h"

int32_t main()
{
    CBasicGameRunner game;
    game.SetBackgroundColor(GRAY)
        .AddKeyboardControlsInfo("Q - Create Attractor at mouse position")
        .AddKeyboardControlsInfo("W - Create Repeller at mouse position")
        .AddKeyboardControlsInfo("E - Clear Emitters")
        .AddKeyboardControlsInfo("Space - Clear Force Fields")
        .AddKeyboardControlsInfo("Left Click - Create emitter at mouse position");

    const auto center = game.GetWindowCenterPosition();

    std::vector<CEmitter> emitters;
    std::vector<std::unique_ptr<IForceField>> forceFields;

    const auto AddInitEmitter = [&]() {
        // Create an initial emitter at the center of the screen.
        emitters.emplace_back(center);
    };

    // Add initial emitter.
    AddInitEmitter();

    auto update = [&]() {
        // Emitter.
        {
            // Create attractor.
            if (IsKeyPressed(KEY_Q)) {
                const auto mousePosition = game.GetMouseWorldPosition();
                auto pAttractor = std::make_unique<CAttractor>(mousePosition, 1000.0f);
                forceFields.emplace_back(std::move(pAttractor));
            }

            // Create repeller.
            if (IsKeyPressed(KEY_W)) {
                const auto mousePosition = game.GetMouseWorldPosition();
                auto pRepeller = std::make_unique<CRepeller>(mousePosition, 1000.0f);
                forceFields.emplace_back(std::move(pRepeller));
            }

            // Remove all emitters.
            if (IsKeyPressed(KEY_E)) {
                emitters.clear();
                emitters.shrink_to_fit();
                AddInitEmitter();
            }

            // Remove all particle force fields.
            if (IsKeyPressed(KEY_SPACE)) {
                forceFields.clear();
                forceFields.shrink_to_fit();
            }

            // Create new emitter.
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                //const auto mousePosition = GetMousePosition();
                const auto mousePosition = game.GetMouseWorldPosition();
                emitters.emplace_back(mousePosition);
            }

            for (auto& emitter : emitters) {
                // Gravity force.
                emitter.ApplyForce(math::Gravity);

                // Force fields.
                for (const auto& forceField : forceFields) {
                    emitter.ApplyForceField(*forceField);
                }

                emitter.Update();
            }
        }
    };

    // Draw world here
    auto drawWorld = [&]() {
        // Force fields.
        for (const auto& forceField : forceFields) {
            forceField->Draw();
        }

        // Emitter.
        for (const auto& emitter : emitters) {
            emitter.Draw();
        }
    };

    auto drawUi = [&] {
        const size_t totalParticleCount = std::accumulate(emitters.begin(), emitters.end(), size_t(),
                            [](size_t sum, const CEmitter& emitter) { return sum + emitter.GetParticleCount(); });

        const size_t totalCapacity = std::accumulate(emitters.begin(), emitters.end(), size_t(),
                            [](size_t sum, const CEmitter& emitter) { return sum + emitter.GetCapacity(); });

        // Particle count.
        const auto text = std::format("Particle Count: {}", totalParticleCount);
        DrawText(text.c_str(), 10, 200, 20, BLACK);
        // Vector capacity.
        const auto capacityText = std::format("Particle Capacity: {}", totalCapacity);
        DrawText(capacityText.c_str(), 10, 220, 20, BLACK);
    };

    // Set callbacks and run the game.
    game.SetUpdateCallback(update)
        .SetDrawWorldCallback(drawWorld)
        .SetDrawUiCallback(drawUi)
        .RunGame();

    return 0;
}
