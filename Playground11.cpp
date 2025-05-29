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

int32_t main()
{
    CBasicGameRunner game;

    const auto center = game.GetWindowCenterPosition();

    auto update = [&]
    {
    };

    // Draw world here
    auto drawWorld = [&]
    {
        const double time = GetTime();
        const float deltaTime = GetFrameTime();

        tool::DrawReferenceCoordinate(game.GetWindowCenterPosition(), time);
    };

    auto drawUi = [&]
    {
    };

    // Set callbacks and run the game.
    game.SetUpdateCallback(update)
        .SetDrawWorldCallback(drawWorld)
        .SetDrawUiCallback(drawUi)
        .RunGame();

    return 0;
}
