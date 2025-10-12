#include "PlaygroundExample.h"

#include <raylib.h>

namespace playground
{
    CPlaygroundExample::CPlaygroundExample(IGame& game) : CPlaygroundBase(game)
    {
    }

    void CPlaygroundExample::Init()
    {
        GetGame().SetCanMoveCameraByMouse(true);
    }

    void CPlaygroundExample::DeInit()
    {
    }

    void CPlaygroundExample::Update()
    {
    }

    void CPlaygroundExample::DrawWorld()
    {
        const auto& [x, y] = GetGame().GetWindowCenterPosition();
        DrawText("This is example", x, y, 20, PINK);
    }

    void CPlaygroundExample::DrawUi()
    {

    }

    std::string_view CPlaygroundExample::GetName() const {
        return "Playground Example";
    }
} // namespace playground
