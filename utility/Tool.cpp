#include "Tool.h"

#include "raymath.h"

void tool::DrawCoordinateChecker(const Vector2& origin, const float time)
{
    constexpr float size = 100.0f;
    const Vector2 xAxisArrowPos = origin + Vector2UnitX * size;
    const Vector2 yAxisArrowPos = origin + Vector2UnitY * size;
    const Vector2 angleArrowPos = origin + Vector2Rotate(Vector2UnitX, time) * size;

    // Draw.
    DrawLineV(origin, xAxisArrowPos, RED);
    DrawLineV(origin, yAxisArrowPos, GREEN);
    DrawLineV(origin, angleArrowPos, BLUE);

    // Text.
    DrawText("+X", static_cast<int>(xAxisArrowPos.x), static_cast<int>(xAxisArrowPos.y), 10, RED);
    DrawText("+Y", static_cast<int>(yAxisArrowPos.x), static_cast<int>(yAxisArrowPos.y), 10, DARKGREEN);
    DrawText("+Angle", static_cast<int>(angleArrowPos.x), static_cast<int>(angleArrowPos.y), 10, BLUE);
}