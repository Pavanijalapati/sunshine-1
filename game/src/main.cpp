#include "raylib.h"
#include <vector>
#include <queue>
#include <iostream>
#include <sstream>

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 1000
#define TILE_SIZE 100

struct Tile {
    Vector2 position;
    Color color;
    bool isWall;
    std::vector<Tile*> connectedTiles;
    float costToReach;
    Tile* previousNode;
    bool visited;
    bool operator==(const Tile& other) const {
        return position.x == other.position.x && position.y == other.position.y;
    }
};

struct Character {
    Texture2D sprite;
    Vector2 position;
    Rectangle bounds;
    float speed;
};

std::vector<Tile> tiles;
Character character;
Tile* startNode;
Tile* goalNode;
Tile* currentNode;
std::vector<Vector2> playerPath;
bool debugMode = true;
bool showBorders = true;
bool showAdjacency = true;
bool showPathInfo = true;

std::string intToString(int value) {
    std::stringstream ss;
    ss << value;
    return ss.str();
}

void generateTiles() {
    const int rows = 8;
    const int cols = 8;

    int totalTiles = rows * cols;
    int wallCount = static_cast<int>(totalTiles * 0.2);

    for (int i = 0; i < totalTiles; ++i) {
        Vector2 position = { 100 + (i % cols) * TILE_SIZE, 100 + (i / cols) * TILE_SIZE };
        Color color = WHITE;  // Set the color to white (255, 255, 255, 255)
        tiles.push_back({ position, color, false, {}, FLT_MAX, nullptr, false });
    }


    for (int i = 0; i < wallCount; ++i) {
        int randomIndex = GetRandomValue(0, totalTiles - 1);
        Tile& tile = tiles[randomIndex];
        if (!tile.isWall) {
            tile.isWall = true;
            tile.color = WHITE;
        }
        else {
            --i;
        }
    }
    for (int i = 0; i < totalTiles; ++i) {
        int row = i / cols;
        int col = i % cols;

        Tile& currentTile = tiles[i];

        if (row > 0) {
            currentTile.connectedTiles.push_back(&tiles[i - cols]); // Add top tile
        }

        if (row < rows - 1) {
            currentTile.connectedTiles.push_back(&tiles[i + cols]); // Add bottom tile
        }

        if (col > 0) {
            currentTile.connectedTiles.push_back(&tiles[i - 1]); // Add left tile
        }

        if (col < cols - 1) {
            currentTile.connectedTiles.push_back(&tiles[i + 1]); // Add right tile
        }
    }
}

void drawAdjacency(const std::vector<Tile>& tiles) {
    const Color lineColor = PINK;
    const Color wallColor = DARKGRAY;

    for (const Tile& tile : tiles) {
        if (tile.isWall) {
            DrawRectangle(static_cast<int>(tile.position.x), static_cast<int>(tile.position.y), TILE_SIZE, TILE_SIZE, wallColor);
        }
        else {
            DrawRectangle(static_cast<int>(tile.position.x), static_cast<int>(tile.position.y), TILE_SIZE, TILE_SIZE, tile.color);
        }

        if (showBorders) {
            DrawRectangleLines(static_cast<int>(tile.position.x), static_cast<int>(tile.position.y), TILE_SIZE, TILE_SIZE, GRAY);
        }

        if (showAdjacency) {
            Vector2 circleCenter = { tile.position.x + TILE_SIZE / 2, tile.position.y + TILE_SIZE / 2 };
            DrawCircle(static_cast<int>(circleCenter.x), static_cast<int>(circleCenter.y), TILE_SIZE / 4, GREEN);

            for (const Tile* connectedTile : tile.connectedTiles) {
                Vector2 lineStart = { circleCenter.x, circleCenter.y };
                Vector2 lineEnd = { connectedTile->position.x + TILE_SIZE / 2, connectedTile->position.y + TILE_SIZE / 2 };
                DrawLineEx(lineStart, lineEnd, 2, lineColor);
            }
        }

        if (showPathInfo) {
            std::string cost = intToString(static_cast<int>(tile.costToReach));
            DrawText(cost.c_str(), static_cast<int>(tile.position.x + TILE_SIZE / 2 - MeasureText(cost.c_str(), 20) / 2),
                static_cast<int>(tile.position.y + TILE_SIZE / 2 - 10), 20,PURPLE );
        }
    }
}

void drawPath(const std::vector<Vector2>& path) {
    if (path.empty()) {
        return;
    }

    for (size_t i = 0; i < path.size() - 1; ++i) {
        Vector2 lineStart = { path[i].x + TILE_SIZE / 2, path[i].y + TILE_SIZE / 2 };
        Vector2 lineEnd = { path[i + 1].x + TILE_SIZE / 2, path[i + 1].y + TILE_SIZE / 2 };
        DrawLineEx(lineStart, lineEnd, 4, BLUE);
    }
}

void resetTiles() {
    for (Tile& tile : tiles) {
        tile.costToReach = FLT_MAX;
        tile.previousNode = nullptr;
        tile.visited = false;
    }
}

float heuristic(const Tile* tile, const Tile* goal) {
  
    return std::abs(tile->position.x - goal->position.x) + std::abs(tile->position.y - goal->position.y);
}

void calculatePath(Tile* start, Tile* goal) {
    resetTiles();

    std::priority_queue<std::pair<float, Tile*>, std::vector<std::pair<float, Tile*>>, std::greater<std::pair<float, Tile*>>> pq;

    start->costToReach = 0.0f;
    pq.push(std::make_pair(start->costToReach + heuristic(start, goal), start));  // Add the heuristic value to the cost

    while (!pq.empty()) {
        Tile* current = pq.top().second;
        pq.pop();

        if (current == goal) {
            break;
        }

        current->visited = true;

        for (Tile* neighbor : current->connectedTiles) {
            if (!neighbor->isWall && !neighbor->visited) {
                float newCost = current->costToReach + 1.0f;

                if (newCost < neighbor->costToReach) {
                    neighbor->costToReach = newCost;
                    neighbor->previousNode = current;
                    pq.push(std::make_pair(newCost + heuristic(neighbor, goal), neighbor));  // Add the heuristic value to the cost
                }
            }
        }
    }
}

void calculatePlayerPath() {
    playerPath.clear();
    Tile* current = goalNode;

    while (current != nullptr) {
        playerPath.push_back(current->position);
        current = current->previousNode;
    }
}

void initializeGame() {
    generateTiles();
    character.position = tiles[0].position;
    character.bounds = { character.position.x, character.position.y, TILE_SIZE, TILE_SIZE };
    startNode = &tiles[0];
    goalNode = &tiles[tiles.size() - 1];
    currentNode = startNode;
    calculatePath(startNode, goalNode);
    calculatePlayerPath();
}

void updateGame() {
    const float speed = 2.0f;

    if (IsKeyPressed(KEY_GRAVE))
        debugMode = !debugMode;

    if (debugMode) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mousePos = GetMousePosition();

            for (Tile& tile : tiles) {
                if (!tile.isWall && CheckCollisionPointRec(mousePos, { tile.position.x, tile.position.y, TILE_SIZE, TILE_SIZE })) {
                    goalNode = &tile;
         
                    calculatePath(startNode, goalNode);
                    calculatePlayerPath();
                    break;
                }
            }
        }

        if (!playerPath.empty()) {
            Vector2 targetPosition = playerPath.back();
            Vector2 moveVector = { targetPosition.x - character.position.x, targetPosition.y - character.position.y };
            float distance = sqrt(moveVector.x * moveVector.x + moveVector.y * moveVector.y);

            if (distance > speed) {
                moveVector.x /= distance;
                moveVector.y /= distance;
                character.position.x += moveVector.x * speed;
                character.position.y += moveVector.y * speed;
            }
            else {
                character.position = targetPosition;
                playerPath.pop_back();
            }

            character.bounds.x = character.position.x;
            character.bounds.y = character.position.y;
        }

        if (IsKeyPressed(KEY_LEFT)) {
            showBorders = !showBorders;
        }

        if (IsKeyPressed(KEY_UP)) {
            showAdjacency = !showAdjacency;
        }

        if (IsKeyPressed(KEY_RIGHT)) {
            showPathInfo = !showPathInfo;
        }
    }

    if (IsKeyDown(KEY_D)) {
        character.position.x += speed;
    }

    if (IsKeyDown(KEY_A)) {
        character.position.x -= speed;
    }

    if (IsKeyDown(KEY_W)) {
        character.position.y -= speed;
    }

    if (IsKeyDown(KEY_S)) {
        character.position.y += speed;
    }

    character.bounds.x = character.position.x;
    character.bounds.y = character.position.y;
}

void drawGame() {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    drawAdjacency(tiles);
    drawPath(playerPath);

    DrawRectangleRec(character.bounds, RED);


    if (debugMode) {
        DrawText("Press SPACE to step through pathfinding", 10, 10, 20, BLACK);
        DrawText("Press LEFT to toggle tile borders", 10, 40, 20, BLACK);
        DrawText("Press UP to toggle tile adjacency", 10, 70, 20, BLACK);
        DrawText("Press RIGHT to toggle path information", 10, 100, 20, BLACK);

        if (currentNode == goalNode) {
            DrawText("Press SPACE to restart", 10, 130, 20, BLACK);
        }
        else if (!playerPath.empty()) {
            DrawText("Press SPACE to step forward", 10, 130, 20, BLACK);
        }
        if (showPathInfo) {
            for (const Tile& tile : tiles) {
                std::string cost = intToString(static_cast<int>(tile.costToReach));
                std::string heuristicValue = intToString(static_cast<int>(heuristic(&tile, goalNode)));
                std::string info = cost + " + " + heuristicValue;

                DrawText(info.c_str(), static_cast<int>(tile.position.x + TILE_SIZE / 2 - MeasureText(info.c_str(), 20) / 2),
                    static_cast<int>(tile.position.y + TILE_SIZE / 2 - 10), 20, BLACK);
            }
        }
    }

    EndDrawing();
}

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pathfinding Algorithm Visualization");
    SetTargetFPS(60);

    

    initializeGame();

    while (!WindowShouldClose()) {
        updateGame();
        drawGame();
    }

    CloseWindow();

    

    return 0;
}
