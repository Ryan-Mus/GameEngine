#pragma once
#include "GhostMovement.h"
#include "PacmanGrid.h"
#include "GameObject.h"
#include <queue>
#include <unordered_map>
#include <algorithm>
#include <iostream>

void GhostMovement::Update()
{
	BaseMovement::Update();
    if (!m_Grid)
        return;

    // Get current position in world coordinates
    glm::vec3 currentWorldPos = GetOwner()->GetWorldPosition();

    // Convert world position to grid coordinates
    auto currentGridPos = m_Grid->WorldToGridPosition(currentWorldPos.x, currentWorldPos.y);

    // Check if current grid position is valid
    if (currentGridPos.first == -1 || currentGridPos.second == -1)
        return;

    // Calculate the next step towards the target
    glm::ivec2 nextStep = CalculateNextStep(currentGridPos, m_Target);

    if (nextStep == glm::ivec2(0, 0))
        return; // No valid path found or already at target

    // Set the movement direction
    glm::vec3 direction = glm::normalize(glm::vec3(nextStep.x, nextStep.y, 0.0f));
    BaseMovement::SetDirection(direction);
}

glm::ivec2 GhostMovement::CalculateNextStep(const std::pair<int, int>& start, const glm::ivec2& goal)
{
    int rows = m_Grid->GetRows();
    int cols = m_Grid->GetColumns();

    // Direction vectors for adjacent cells (up, down, left, right)
    std::vector<glm::ivec2> directions = {
        {0, 1}, // Up
        {0, -1},  // Down
        {-1, 0}, // Left
        {1, 0}   // Right
    };

    // Queue for BFS frontier
    std::queue<std::pair<int, int>> frontier;
    frontier.push(start);

    // Maps grid positions to their previous positions
    std::unordered_map<int, std::pair<int, int>> cameFrom;
    auto encode = [cols](int x, int y) { return y * cols + x; };
    cameFrom[encode(start.first, start.second)] = { -1, -1 };

    bool found = false;

    while (!frontier.empty())
    {
        auto current = frontier.front();
        frontier.pop();

        if (current.first == goal.x && current.second == goal.y)
        {
            found = true;
            break;
        }

        for (const auto& dir : directions)
        {
            int nextX = current.first + dir.x;
            int nextY = current.second + dir.y;

            // Check bounds and passable cells
            if (nextX < 0 || nextX >= cols || nextY < 0 || nextY >= rows)
                continue;
            if (m_Grid->GetCellType(nextX, nextY) == CellType::Wall)
                continue;

            int encoded = encode(nextX, nextY);
            if (cameFrom.find(encoded) == cameFrom.end())
            {
                frontier.push({ nextX, nextY });
                cameFrom[encoded] = current;
            }
        }
    }

    if (!found)
        return { 0, 0 }; // No path found

    // Reconstruct path from goal to start
    std::vector<std::pair<int, int>> path;
    auto current = std::make_pair(goal.x, goal.y);

    while (current != start)
    {
        path.push_back(current);
        current = cameFrom[encode(current.first, current.second)];
    }

    std::reverse(path.begin(), path.end());

    if (path.empty())
        return { 0, 0 }; // Already at target

    // Next step direction
    auto nextCell = path.front();
    return { nextCell.first - start.first, nextCell.second - start.second };
}
