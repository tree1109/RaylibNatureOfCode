#include "PathFinder.h"

#include <numbers>
#include <unordered_map>
#include <memory>
#include <deque>
#include <queue>
#include <stdexcept>

#include "CommonType.h"

bool PathFinder::NodeComparator::operator()(const std::shared_ptr<Node>& lhs, const std::shared_ptr<Node>& rhs) const
{
    return lhs->F > rhs->F;
}

std::vector<Vector2I> PathFinder::AStar(Vector2I start, Vector2I end) const
{
    // Skip if the start or end point is out of the grid.
    if (start.x < 0 || start.x >= m_width || start.y < 0 || start.y >= m_height)
    {
        return {};
    }
    if (end.x < 0 || end.x >= m_width || end.y < 0 || end.y >= m_height)
    {
        return {};
    }

    // Open list.
    std::unordered_map<uint64_t, std::shared_ptr<Node>> openList;
    std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, NodeComparator> openQueue;

    // Close list.
    std::unordered_map<uint64_t, std::shared_ptr<Node>> closeList;

    // Start node.
    std::shared_ptr<Node> pStartNode = std::make_shared<Node>(start);
    openList.insert({ GetHashKey(pStartNode->Point), pStartNode });
    openQueue.push(pStartNode);

    // Loop until the open list is empty.
    while (!openList.empty())
    {
        // Get the node with the lowest FCost.
        std::shared_ptr<Node> pCurrentNode = openQueue.top();
        openQueue.pop();
        const Vector2I& currentPoint = pCurrentNode->Point;

        // Remove the node from the open list to the close list.
        openList.erase(GetHashKey(currentPoint));
        closeList.insert({ GetHashKey(currentPoint), pCurrentNode });

        // Construct path if reached the end node.
        if (currentPoint == end)
        {
            return ReconstructPath(pCurrentNode);
        }

        // Get the neighbors.
        std::vector<Vector2I> neighbors;
        neighbors.reserve(8);
        for (int32_t y = -1; y <= 1; ++y)
        {
            for (int32_t x = -1; x <= 1; ++x)
            {
                // Skip the current node.
                if (x == 0 && y == 0)
                {
                    continue;
                }
                // Calculate the neighbor.
                Vector2I neighbor = { currentPoint.x + x, currentPoint.y + y };
                // Skip if the neighbor is out of the grid.
                if (neighbor.x < 0 || neighbor.x >= m_width || neighbor.y < 0 || neighbor.y >= m_height)
                {
                    continue;
                }
                // Skip if the neighbor is an obstacle.
                if (m_grid[GetIndex(neighbor)] > 0)
                {
                    continue;
                }
                neighbors.push_back(neighbor);
            }
        }

        // Process the neighbors.
        for (const Vector2I& neighbor : neighbors)
        {
            // Skip if the neighbor is in the close list.
            if (closeList.contains(GetHashKey(neighbor)))
            {
                continue;
            }
            // Calculate the cost.
            const bool isDiagonal = (abs(neighbor.x - currentPoint.x) + abs(neighbor.y - currentPoint.y)) == 2;
            float g = pCurrentNode->G + (isDiagonal ? std::numbers::sqrt2_v<float> : 1.0f);
            float h = static_cast<float>(abs(neighbor.x - end.x) + abs(neighbor.y - end.y));
            float f = g + h;


            auto it = openList.find(GetHashKey(neighbor));

            // If not in the open list, add it.
            if (it == openList.end())
            {
                // Create the neighbor node.
                auto pNeighborNode = std::make_shared<Node>(neighbor, pCurrentNode, f, g, h);
                openList.insert({ GetHashKey(neighbor), pNeighborNode });
                openQueue.push(pNeighborNode);
                continue;
            }
            // If the neighbor is in the open list.
            auto pNeighborNode = it->second;

            // Skip if the cost is higher.
            if (pNeighborNode->F < f)
            {
                continue;
            }
            // Update the neighbor node.
            pNeighborNode->F = f;
            pNeighborNode->G = g;
            pNeighborNode->H = h;
            pNeighborNode->Parent = pCurrentNode;
        }
    }

    return {};
}

uint64_t PathFinder::GetHashKey(Vector2I point)
{
    uint64_t x = static_cast<uint64_t>(point.x) << 32;
    uint64_t y = static_cast<uint64_t>(point.y);
    x &= 0xFFFFFFFF00000000;
    y &= 0x00000000FFFFFFFF;
    return x | y;
}

std::vector<Vector2I> PathFinder::ReconstructPath(const std::shared_ptr<Node>& pEndNode)
{
    std::vector<Vector2I> path;
    std::shared_ptr<Node> pNode = pEndNode;
    while (pNode != nullptr)
    {
        path.push_back(pNode->Point);

        if (auto pParent = pNode->Parent.lock(); pParent != nullptr)
        {
            pNode = pParent;
        }
        else
        {
            pNode = nullptr;
        }
    }
    return { path.rbegin(), path.rend() };
}

size_t PathFinder::GetIndex(Vector2I point) const
{
    return point.y * m_width + point.x;
}