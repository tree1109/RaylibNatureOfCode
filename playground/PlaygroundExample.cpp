#include "PlaygroundExample.h"

#include <raylib.h>

namespace playground
{
    CPlaygroundExample::CPlaygroundExample(IGame& game) : m_game(game)
    {
    }

    void CPlaygroundExample::Init()
    {
    }

    void CPlaygroundExample::DeInit()
    {
    }

    void CPlaygroundExample::Update()
    {
    }

    void CPlaygroundExample::DrawWorld()
    {
        const auto& [x, y] = m_game.GetWindowCenterPosition();
        DrawText("This is example", x, y, 20, PINK);
    }

    void CPlaygroundExample::DrawUi()
    {
        const auto& [x, y] = m_game.GetWindowCenterPosition();
        DrawText("UwU", x, y, 40, RED);
    }

    std::string_view CPlaygroundExample::GetName() const {
        return "Playground Example";
    }
} // namespace playground
