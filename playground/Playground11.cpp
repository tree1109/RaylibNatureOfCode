#include "Playground11.h"

#include <format>

#include "game_object/Attractor.h"
#include "game_object/Emitter.h"
#include "game_object/Repeller.h"
#include "raymath.h"
#include "utility/Math.h"

namespace
{
    std::vector<CEmitter> emitters;
    std::vector<std::unique_ptr<IForceField>> forceFields;

    void AddInitEmitter(const Vector2& center)
    {
        // Create an initial emitter at the center of the screen.
        emitters.emplace_back(center);
        emitters.back().SetRandomColor(true);
    }
} // namespace

namespace playground
{
    CPlayground11::CPlayground11(IGame& game) : m_game(game)
    {
    }

    void CPlayground11::Init()
    {
        m_game.SetBackgroundColor(GRAY)
            .AddKeyboardControlsInfo("Q - Create Attractor at mouse position")
            .AddKeyboardControlsInfo("W - Create Repeller at mouse position")
            .AddKeyboardControlsInfo("E - Clear Emitters")
            .AddKeyboardControlsInfo("Space - Clear Force Fields")
            .AddKeyboardControlsInfo("Left Click - Create emitter at mouse position");


        const auto& center = m_game.GetWindowCenterPosition();

        // Add initial emitter.
        AddInitEmitter(center);
    }

    void CPlayground11::DeInit()
    {
    }

    void CPlayground11::Update()
    {
        const auto& center = m_game.GetWindowCenterPosition();
        const auto& mousePosition = m_game.GetMouseWorldPosition();

        // Emitter.
        {
            // Create attractor.
            if (IsKeyPressed(KEY_Q)) {
                auto pAttractor = std::make_unique<CAttractor>(mousePosition, 1000.0f);
                forceFields.emplace_back(std::move(pAttractor));
            }

            // Create repeller.
            if (IsKeyPressed(KEY_W)) {
                auto pRepeller = std::make_unique<CRepeller>(mousePosition, 1000.0f);
                forceFields.emplace_back(std::move(pRepeller));
            }

            // Remove all emitters.
            if (IsKeyPressed(KEY_E)) {
                emitters.clear();
                emitters.shrink_to_fit();
                AddInitEmitter(center);
            }

            // Remove all particle force fields.
            if (IsKeyPressed(KEY_SPACE)) {
                forceFields.clear();
                forceFields.shrink_to_fit();
            }

            // Create new emitter.
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                // const auto mousePosition = GetMousePosition();
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
    }

    void CPlayground11::DrawWorld()
    {
        // Force fields.
        for (const auto& forceField : forceFields) {
            forceField->Draw();
        }

        // Emitter.
        for (const auto& emitter : emitters) {
            emitter.Draw();
        }
    }

    void CPlayground11::DrawUi()
    {
        const size_t totalParticleCount =
            std::accumulate(emitters.begin(), emitters.end(), size_t(),
                            [](size_t sum, const CEmitter& emitter) { return sum + emitter.GetParticleCount(); });

        const size_t totalCapacity =
            std::accumulate(emitters.begin(), emitters.end(), size_t(),
                            [](size_t sum, const CEmitter& emitter) { return sum + emitter.GetCapacity(); });

        // Particle count.
        const auto text = std::format("Particle Count: {}", totalParticleCount);
        DrawText(text.c_str(), 10, 240, 20, BLACK);
        // Vector capacity.
        const auto capacityText = std::format("Particle Capacity: {}", totalCapacity);
        DrawText(capacityText.c_str(), 10, 260, 20, BLACK);
    }

    std::string_view CPlayground11::GetName() const
    {
        return "Playground 11";
    }
} // namespace playground
