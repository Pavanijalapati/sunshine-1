#pragma once
#include "Tilemap.h"
#include <unordered_map>
#include <vector>
#include "raylib.h"
#include <string>

class Pathfinder
{
public:
    Tilemap* map = nullptr;

private:
    std::unordered_map<TileCoord, float> unvisited;
    std::unordered_map<TileCoord, float> visited;
    std::unordered_map<TileCoord, TileCoord> cheapestEdgeTo;

    TileCoord startNode;
    TileCoord goalNode;
    TileCoord currentNode;
    std::vector<TileCoord> solutionPath;

public:
    Pathfinder() {}

    Pathfinder(Tilemap* levelToNavigate, TileCoord startTile, TileCoord endTile)
    {
        map = levelToNavigate;
        startNode = startTile;
        goalNode = endTile;
        currentNode = startNode;
        Restart();
    }

    void Restart()
    {
        visited.clear();
        cheapestEdgeTo.clear();
        unvisited.clear();

        unvisited.clear();
        for (const TileCoord& position : map->GetAllTraversableTiles())
        {
            unvisited[position] = INFINITY;
        }
        unvisited[startNode] = 0;
    }

    void SetStartAndGoal(TileCoord startTile, TileCoord endTile)
    {
        startNode = startTile;
        goalNode = endTile;
        currentNode = startNode;
        Restart();
    }

    void Step()
    {
        if (unvisited.empty())
        {
            // No path found, algorithm ends in failure
            return;
        }

        // Get the node with the lowest cost from the unvisited set
        TileCoord lowestCostNode;
        float lowestCost = INFINITY;
        for (const auto& node : unvisited)
        {
            TileCoord position = node.first;
            float cost = node.second;
            if (cost < lowestCost)
            {
                lowestCost = cost;
                lowestCostNode = position;
            }
        }

        if (lowestCostNode == goalNode)
        {
            // Goal node reached, algorithm ends in success
            return;
        }

        // Move the lowest cost node from the unvisited set to the visited set
        unvisited.erase(lowestCostNode);
        visited[lowestCostNode] = lowestCost;

        // Evaluate edges to neighbor nodes from the current node
        std::vector<TileCoord> neighbors = map->GetTraversableTilesAdjacentTo(lowestCostNode);
        for (const auto& neighbor : neighbors)
        {
            float costToNeighbor = lowestCost + map->GetCostForTile(neighbor);
            if (costToNeighbor < unvisited[neighbor])
            {
                unvisited[neighbor] = costToNeighbor;
                cheapestEdgeTo[neighbor] = lowestCostNode;
            }
        }
    }

    void Solve()
    {
        while (!unvisited.empty() && currentNode != goalNode)
        {
            Step();
            currentNode = GetLowestCostNode();
            solutionPath = GenerateSolutionPath();
        }
    }

    TileCoord GetLowestCostNode()
    {
        TileCoord lowestCostNode;
        float lowestCost = INFINITY;
        for (const auto& node : unvisited)
        {
            TileCoord position = node.first;
            float cost = node.second;
            if (cost < lowestCost)
            {
                lowestCost = cost;
                lowestCostNode = position;
            }
        }
        return lowestCostNode;
    }

    std::vector<TileCoord> GenerateSolutionPath()
    {
        std::vector<TileCoord> path;
        TileCoord current = goalNode;
        while (current != startNode)
        {
            path.push_back(current);
            if (cheapestEdgeTo.find(current) != cheapestEdgeTo.end())
            {
                current = cheapestEdgeTo[current];
            }
            else
            {
                // No valid path exists
                path.clear();
                break;
            }
        }
        std::reverse(path.begin(), path.end());
        return path;
    }

    std::vector<TileCoord> GetSolutionPath()
    {
        return solutionPath;
    }

    void DrawDebugInfo()
    {
        // Draw nodes in unvisited set with cost-to-reach in gray
        for (const auto& node : unvisited)
        {
            TileCoord position = node.first;
            float cost = node.second;
            DrawText(std::to_string(static_cast<int>(cost)).c_str(), position.x * map->tileSizeX, position.y * map->tileSizeY, 12, GRAY);
        }

        // Draw nodes in visited set with cost-to-reach in green
        for (const auto& node : visited)
        {
            TileCoord position = node.first;
            float cost = node.second;
            DrawText(std::to_string(static_cast<int>(cost)).c_str(), position.x * map->tileSizeX, position.y * map->tileSizeY, 12, GREEN);
        }

        // Draw start node in green
        DrawRectangle(startNode.x * map->tileSizeX, startNode.y * map->tileSizeY, map->tileSizeX, map->tileSizeY, GREEN);

        // Draw goal node in red
        DrawRectangle(goalNode.x * map->tileSizeX, goalNode.y * map->tileSizeY, map->tileSizeX, map->tileSizeY, RED);

        // Draw current node in blue
        DrawRectangle(currentNode.x * map->tileSizeX, currentNode.y * map->tileSizeY, map->tileSizeX, map->tileSizeY, BLUE);

        // Draw a line in green from each node to its previous node
        for (const auto& edge : cheapestEdgeTo)
        {
            TileCoord from = edge.first;
            TileCoord to = edge.second;
            Vector2 fromPos = map->GetScreenPosOfTileCentered(from);
            Vector2 toPos = map->GetScreenPosOfTileCentered(to);
            DrawLineEx(fromPos, toPos, 2, GREEN);
        }

        // Draw the solution path in blue
        for (size_t i = 0; i < solutionPath.size() - 1; ++i)
        {
            TileCoord from = solutionPath[i];
            TileCoord to = solutionPath[i + 1];
            Vector2 fromPos = map->GetScreenPosOfTileCentered(from);
            Vector2 toPos = map->GetScreenPosOfTileCentered(to);
            DrawLineEx(fromPos, toPos, 2, BLUE);
        }
    }
};
