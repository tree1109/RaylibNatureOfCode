#include "Tool.h"

#include "raymath.h"

void tool::DrawReferenceCoordinate(const Vector2& origin, const float time)
{
    constexpr float size = 100.0f;
    constexpr float thickness = 1.0f;
    const Vector2 xAxisArrowPos = origin + Vector2UnitX * size;
    const Vector2 yAxisArrowPos = origin + Vector2UnitY * size;
    const Vector2 negativeXAxisArrowPos = origin - Vector2UnitX * size;
    const Vector2 negativeYAxisArrowPos = origin - Vector2UnitY * size;
    const Vector2 angleArrowPos = origin + Vector2Rotate(Vector2UnitX, time) * size;

    // x-axis.
    DrawLineEx(origin, xAxisArrowPos, thickness, RED);
    DrawLineEx(origin, negativeXAxisArrowPos, thickness, MAROON);
    const Vector2 xAxisArrowPos1 = xAxisArrowPos + Vector2{ -10.0f, -5.0f };
    const Vector2 xAxisArrowPos2 = xAxisArrowPos + Vector2{ -10.0f, 5.0f };
    DrawTriangle(xAxisArrowPos, xAxisArrowPos1, xAxisArrowPos2, RED);

    // y-axis.
    DrawLineEx(origin, yAxisArrowPos, thickness, LIME);
    DrawLineEx(origin, negativeYAxisArrowPos, thickness, DARKGREEN);
    const Vector2 yAxisArrowPos1 = yAxisArrowPos + Vector2{ -5.0f, -10.0f };
    const Vector2 yAxisArrowPos2 = yAxisArrowPos + Vector2{ 5.0f, -10.0f };
    DrawTriangle(yAxisArrowPos, yAxisArrowPos2, yAxisArrowPos1, LIME);

    // angle.
    DrawLineEx(origin, angleArrowPos, thickness, BLUE);
    DrawCircleLinesV(origin, size, DARKBLUE);

    DrawText("+X", static_cast<int>(xAxisArrowPos.x), static_cast<int>(xAxisArrowPos.y), 20, RED);
    DrawText("+Y", static_cast<int>(yAxisArrowPos.x), static_cast<int>(yAxisArrowPos.y), 20, LIME);
    DrawText("+Angle", static_cast<int>(angleArrowPos.x), static_cast<int>(angleArrowPos.y), 20, BLUE);
    DrawText(TextFormat("%.0f, %.0f", origin.x, origin.y), static_cast<int>(origin.x), static_cast<int>(origin.y), 10, BLACK);
}
