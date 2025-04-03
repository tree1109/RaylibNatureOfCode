#include <raylib.h>
#include <raymath.h>
#include <cstdint>
#include <functional>

#include "utility/BasicGameRunner.h"
#include "utility/Tool.h"

int32_t main()
{
    CBasicGameRunner game;

    const auto center = game.GetWindowCenterPosition();

    game.SetUpdateCallback([]() {
        // Update logic here
    });

    game.SetWorldDrawCallback([&]() {
        const float time = static_cast<float>(GetTime());

        // Draw world here
        tool::DrawReferenceCoordinate(center, time);
    });

    game.SetUIDrawCallback([]() {
        // Draw UI here

    });

    game.RunGame();

    return 0;
}
