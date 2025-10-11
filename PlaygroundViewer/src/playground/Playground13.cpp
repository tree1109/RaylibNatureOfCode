#include "Playground13.h"

#include "IGame.h"
#include "game_object/Mover.h"
#include "raymath.h"

namespace
{

    CMover vehicle;
}

namespace playground
{
    CPlayground13::CPlayground13(IGame& game)
        : m_game(game)
    {

    }

    void CPlayground13::Init()
    {
        // Get center.
        const auto& center = m_game.GetWindowCenterPosition();
        vehicle = CMover();
        vehicle.SetPosition(center);
    }

    void CPlayground13::DeInit()
    {
    }

    void CPlayground13::Update()
    {
        // Use mouse as target.
        const auto& mousePosition = m_game.GetMouseWorldPosition();
        {
            const auto& target = mousePosition;
            const auto desired = target - vehicle.GetPosition();
            const auto steer = desired - vehicle.GetVelocity();
            vehicle.ApplyForce(steer);
        }

        vehicle.Update();
    }

    void CPlayground13::DrawWorld()
    {
        {
            const auto& mousePosition = m_game.GetMouseWorldPosition();
            DrawCircleV(mousePosition, 10.0f, RED);
        }

        vehicle.Draw();
    }

    void CPlayground13::DrawUi()
    {
    }

    std::string_view CPlayground13::GetName() const
    {
        return "Playground 13";
    }
} // namespace playground
