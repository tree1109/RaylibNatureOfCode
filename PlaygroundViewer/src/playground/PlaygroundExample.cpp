#include "PlaygroundExample.h"

#include <raylib.h>

namespace playground
{
    CPlaygroundExample::CPlaygroundExample(IGame& game) : m_game(game)
    {
    }

    void CPlaygroundExample::Init()
    {
        m_game.SetCanMoveCameraByMouse(true);
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

    }

    std::string_view CPlaygroundExample::GetName() const {
        return "Playground Example";
    }
} // namespace playground
