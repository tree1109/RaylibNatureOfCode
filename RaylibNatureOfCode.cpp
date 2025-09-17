#include <raylib.h>

#include <format>
#include <functional>
#include <type_traits>

#include "playground/PlaygroundExample.h"
#include "playground/IPlayground.h"
#include "utility/BasicGameRunner.h"

// Template playground.
void NormalPlayground()
{
    CBasicGameRunner game;

    const auto center = game.GetWindowCenterPosition();

    // Init here.
    auto init = [&]() {
    };

    // DeInit here.
    auto deInit = [&]() {
    };

    // Update logic here
    auto update = [&] {
    };

    // Draw world here
    auto drawWorld = [&] {
        const double time = GetTime();
        const float deltaTime = GetFrameTime();
    };

    // Draw UI here
    auto drawUi = [&] {
    };

    // Set callbacks and run the game.
    game.SetInitCallback(init)
        .SetDeInitCallback(deInit)
        .SetUpdateCallback(update)
        .SetDrawWorldCallback(drawWorld)
        .SetDrawUiCallback(drawUi)
        .RunGame();
}

namespace
{
    template <typename T>
    concept PlaygroundType = std::is_base_of_v<playground::IPlayground, T>;

    template <PlaygroundType T>
    void AddPlayground(CBasicGameRunner& game)
    {
        game.AddPlayground(std::make_unique<T>(game));
    }
}

int32_t main()
{
    CBasicGameRunner game;

    // Service.

    // Manager.


    // Playground.
    AddPlayground<playground::CPlaygroundExample>(game);

    // Set callbacks and run the game.
    game.RunGame();

    return 0;
}
