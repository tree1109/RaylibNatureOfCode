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

#include "utility/BasicGameRunner.h"
#include "utility/Math.h"
#include "utility/Tool.h"

#include "game_object/Particle.h"

int32_t main()
{
    CBasicGameRunner game;
    game.SetBackgroundColor(GRAY);

    const auto center = game.GetWindowCenterPosition();

    std::vector<CParticle> particles;

    auto update = [&]() {
        particles.erase(std::ranges::remove_if(particles, [](const CParticle& particle) {
            return particle.IsDead();
        }).begin(), particles.end());

        constexpr int32_t kParticleCountSpawnPerFrame = 10;
        for (int32_t i = 0; i < kParticleCountSpawnPerFrame; i++) {
            particles.emplace_back(center);
        }

        particles.emplace_back(center);

        for(auto& particle : particles) {
            constexpr Vector2 gravity { 0.0f, 9.81f };

            const Vector2 randomForce = Vector2{
                math::GetRandomGaussian(0.0f, 10.0f),
                math::GetRandomGaussian(0.0f, 10.0f)
            };

            particle.ApplyForce(gravity);
            particle.ApplyForce(randomForce);
            particle.Update();
        }
    };

    // Draw world here
    auto drawWorld = [&]() {
        const double time = GetTime();
        const float deltaTime = GetFrameTime();

        for (const auto& particle : particles)
        {
            particle.Draw();
        }
    };

    auto drawUi = [&]
    {
        // Particle count.
        const auto text = std::format("Particle Count: {}", particles.size());
        DrawText(text.c_str(), 10, 100, 20, BLACK);
        // Vector capacity.
        const auto capacityText = std::format("Particle Capacity: {}", particles.capacity());
        DrawText(capacityText.c_str(), 10, 120, 20, BLACK);
    };

    // Set callbacks and run the game.
    game.SetUpdateCallback(update)
        .SetDrawWorldCallback(drawWorld)
        .SetDrawUiCallback(drawUi)
        .RunGame();

    return 0;
}
