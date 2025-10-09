#include "Tool.h"

#include <raymath.h>
#include <cmath>
#include <stdexcept>

#include "SafeImage.h"

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

    constexpr int fontSize = 10;

    DrawText("+X", static_cast<int>(xAxisArrowPos.x), static_cast<int>(xAxisArrowPos.y), fontSize, RED);
    DrawText("+Y", static_cast<int>(yAxisArrowPos.x), static_cast<int>(yAxisArrowPos.y), fontSize, LIME);
    DrawText("+Angle", static_cast<int>(angleArrowPos.x), static_cast<int>(angleArrowPos.y), fontSize, BLUE);

    const int infoX = static_cast<int>(origin.x) + fontSize;
    const int infoY = static_cast<int>(origin.y) + fontSize;
    const int degrees = static_cast<int>(RAD2DEG * time) % 360;

    DrawText(TextFormat("X: %d", infoX), infoX, infoY, fontSize, RED);
    DrawText(TextFormat("Y: %d", infoY), infoX, (infoY + fontSize), fontSize, LIME);
    DrawText(TextFormat("Deg: %d", degrees), infoX, (infoY + fontSize * 2), fontSize, BLUE);
}

CSafeImage tool::GenerateBlurCircleImage(const Color& color, const float maxAlpha)
{
    constexpr int32_t size = 64;
    Image image = GenImageColor(size, size, BLANK);

    constexpr int32_t cx = size / 2;
    constexpr int32_t cy = size / 2;
    constexpr float radius = size / 2.0f;

    for (int32_t y = 0; y < size; y++) {
        for (int32_t x = 0; x < size; x++) {
            const float dx = static_cast<float>(x) - cx;
            const float dy = static_cast<float>(y) - cy;
            const float dist = sqrtf(dx * dx + dy * dy);
            const float t = 1.0f - (dist / radius);
            if (t > 0) {
                const float alpha = std::sin(t) * maxAlpha;
                const Color col = ColorAlpha(color, alpha);
                ImageDrawPixel(&image, x, y, col);
            }
        }
    }

    return CSafeImage{image};
}
