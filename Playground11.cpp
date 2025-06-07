#include <raylib.h>
#include <raymath.h>

#include <algorithm>
#include <array>
#include <vector>
#include <deque>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <cmath>
#include <concepts>
#include <cstdint>
#include <functional>
#include <iostream>
#include <memory>
#include <numbers>
#include <random>
#include <stdexcept>
#include <string>
#include <utility>

#include "game_object/Emitter.h"
#include "utility/BasicGameRunner.h"
#include "utility/Math.h"
#include "utility/Tool.h"

#include "game_object/Particle.h"

int32_t main()
{
    CBasicGameRunner game;
    game.SetBackgroundColor(GRAY)
        .AddKeyboardControlsInfo("E - Clear Emitters")
        .AddKeyboardControlsInfo("Left Click - Create Emitter");

    const auto center = game.GetWindowCenterPosition();

    std::vector<CParticle> particles;
    std::vector<CEmitter> emitters;

    auto update = [&]() {
        // Particles.
        {
            particles.erase(std::ranges::remove_if(particles, [](const CParticle &particle) {
                return particle.IsDead();
            }).begin(), particles.end());

            constexpr int32_t kParticleCountSpawnPerFrame = 10;
            for (int32_t i = 0; i < kParticleCountSpawnPerFrame; i++) {
                particles.emplace_back(center);
            }

            particles.emplace_back(center);

            for (auto &particle: particles) {
                constexpr Vector2 gravity{0.0f, 9.81f};

                const Vector2 randomForce = Vector2{
                    math::GetRandomGaussian(0.0f, 10.0f),
                    math::GetRandomGaussian(0.0f, 10.0f)
            };

                particle.ApplyForce(gravity);
                particle.ApplyForce(randomForce);
                particle.Update();
            }
        }

        // Emitter.
        {
            // Remove all emitters.
            if (IsKeyPressed(KEY_E)) {
                emitters.clear();
                emitters.shrink_to_fit();
            }

            // Create new emitter.
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                const auto mousePosition = GetMousePosition();
                emitters.emplace_back(mousePosition);
                emitters.back()
                    .SetParticleForce(math::Gravity)
                    .SetParticleLifeTime(5.0f)
                    .SetParticleInitialForce(math::GetRandomDirection() * 1000.0f);
            }

            for (auto& emitter : emitters) {
                emitter.Update();
            }
        }
    };

    // Draw world here
    auto drawWorld = [&]() {
        // Particles.
        for (const auto& particle : particles) {
            particle.Draw();
        }

        // Emitter.
        for (const auto& emitter : emitters) {
            emitter.Draw();
        }
    };

    auto drawUi = [&]
    {
        const size_t totalParticleCount = particles.size() + std::accumulate(emitters.begin(), emitters.end(), 0,
            [](size_t sum, const CEmitter& emitter) { return sum + emitter.GetParticleCount(); });

        const size_t totalCapacity = particles.capacity() + std::accumulate(emitters.begin(), emitters.end(), 0,
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
