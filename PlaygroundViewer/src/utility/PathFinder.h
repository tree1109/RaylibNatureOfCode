#pragma once

#ifndef INCLUDE_PATH_FINDER_H
#define INCLUDE_PATH_FINDER_H

#include <memory>
#include <stdexcept>
#include <vector>

#include "CommonType.h"

struct Vector2I;

class PathFinder
{
private:
    struct NodeComparator
    {
        bool operator()(const std::shared_ptr<Node>& lhs, const std::shared_ptr<Node>& rhs) const;
    };

public:
    PathFinder(std::vector<int32_t>&& grid, int32_t width, int32_t height)
        : m_grid(std::move(grid))
        , m_width(width)
        , m_height(height)
    {
    }

public:
    std::vector<Vector2I> AStar(Vector2I start, Vector2I end) const;

private:
    static uint64_t GetHashKey(Vector2I point);
    static std::vector<Vector2I> ReconstructPath(const std::shared_ptr<Node>& pEndNode);

    size_t GetIndex(Vector2I point) const;

private:
    std::vector<int32_t> m_grid;
    int32_t m_width;
    int32_t m_height;
};

#endif // !INCLUDE_PATH_FINDER_H
