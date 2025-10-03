#pragma once

#ifndef INCLUDE_COMMON_TYPE_H
#define INCLUDE_COMMON_TYPE_H

#include <cstdint>

struct Vector2I
{
    int32_t x = 0;
    int32_t y = 0;
};

inline bool operator==(const Vector2I& lhs, const Vector2I& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

struct Node
{
    Node(Vector2I point, const std::shared_ptr<Node>& pParent = nullptr, float f = 0.0f, float g = 0.0f, float h = 0.0f)
        : Point(point)
        , F(f)
        , G(g)
        , H(h)
        , Parent(pParent)
    {
    }

    Vector2I Point;
    float F = 0.0f;
    float G = 0.0f;
    float H = 0.0f;
    std::weak_ptr<Node> Parent;
};

#endif // !INCLUDE_COMMON_TYPE_H
