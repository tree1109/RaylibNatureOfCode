#include <raylib.h>

#include <format>
#include <functional>
#include <type_traits>

#include "playground/IPlayground.h"
#include "playground/Playground1.h"
#include "playground/Playground2.h"
#include "playground/Playground3.h"
#include "playground/Playground4.h"
#include "playground/Playground5.h"
#include "playground/Playground6.h"
#include "playground/Playground7.h"
#include "playground/Playground8.h"
#include "playground/Playground9.h"
#include "playground/Playground10.h"
#include "playground/Playground11.h"
#include "playground/Playground12.h"
#include "playground/PlaygroundExample.h"
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
    {

    }

    // Manager.
    {

    }

    // Playground.
    {
        AddPlayground<playground::CPlaygroundExample>(game);
        AddPlayground<playground::CPlayground1>(game);
        AddPlayground<playground::CPlayground2>(game);
        AddPlayground<playground::CPlayground3>(game);
        AddPlayground<playground::CPlayground4>(game);
        AddPlayground<playground::CPlayground5>(game);
        AddPlayground<playground::CPlayground6>(game);
        AddPlayground<playground::CPlayground7>(game);
        AddPlayground<playground::CPlayground8>(game);
        AddPlayground<playground::CPlayground9>(game);
        AddPlayground<playground::CPlayground10>(game);
        AddPlayground<playground::CPlayground11>(game);
        AddPlayground<playground::CPlayground12>(game);
    }

    // Switch playground.
    const auto& update = [&]() {
        if (IsKeyPressed(KEY_LEFT)) {
            game.PreviousPlayground();
        }
        else if (IsKeyPressed(KEY_RIGHT)) {
            game.NextPlayground();
        }
    };

    // Info.
    game.AddKeyboardControlsInfo("[<-/->]: Switch Playground");

    // Set callbacks and run the game.
    game.SetUpdateCallback(update)
        .RunGame();

    return 0;
}
