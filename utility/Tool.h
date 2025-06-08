#pragma once

#ifndef INCLUDE_TOOL_H
#define INCLUDE_TOOL_H

#include <raylib.h>
#include <cstdint>

class CSafeImage;

namespace tool {
    void DrawReferenceCoordinate(const Vector2& origin, const float time);

    CSafeImage GenerateBlurCircleImage(const Color& color);
}

#endif // !INCLUDE_TOOL_H
