#include "Playground12.h"

#include "game_object/Emitter.h"
#include "game_object/RandomField.h"
#include "utility/Math.h"
#include "utility/SafeImage.h"
#include "utility/TextureManager.h"
#include "utility/Tool.h"

namespace
{
    std::vector<CEmitter> fireEmitters;
    CRandomField randomField{100.0f};

    CTextureManager textureManager;

    void Reset(const IGame& game)
    {
        const auto& center = game.GetWindowCenterPosition();

        fireEmitters.clear();
        fireEmitters.emplace_back(center);

        fireEmitters.back()
            .SetRandomColor(true)
            .SetTint(PINK)
            .SetDrawParticleWithTexture(true)
            .SetParticleTexture(textureManager.GetTexture("fire"))
            .SetParticleSpawnCountPerFrame(1);
    }
} // namespace

namespace playground
{
    CPlayground12::CPlayground12(IGame& game) : m_game(game)
    {
    }

    void CPlayground12::Init()
    {
        m_game.SetBackgroundColor(BLACK)
            .SetInfoFontColor(RAYWHITE)
            .AddKeyboardControlsInfo("E - Clear Emitters")
            .AddKeyboardControlsInfo("Left Click - Blow wind according mouse position");

        randomField = {100.0f};

        const auto fireImage = tool::GenerateBlurCircleImage(WHITE, 0.1f);
        textureManager.LoadTexture("fire", fireImage);

        Reset(m_game);
    }

    void CPlayground12::DeInit()
    {
        textureManager.UnloadTextures();
    }

    void CPlayground12::Update()
    {
        if (IsKeyPressed(KEY_E)) {
            Reset(m_game);
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
    }

    void CPlayground12::DrawWorld()
    {
        BeginBlendMode(BLEND_ADDITIVE);
        for (const auto& emitter : fireEmitters) {
            emitter.Draw();
        }
        EndBlendMode();
    }

    void CPlayground12::DrawUi()
    {
    }

    std::string_view CPlayground12::GetName() const
    {
        return "Playground 12";
    }
} // namespace playground
