#include <iostream>
#include <vector>
#include "raylib.h"

const int TILE_SIZE = 50;

enum class TileType
{
    Floor,
    Wall
};

struct Tile
{
    TileType type;
    std::vector<int> adjacencyList;
};

class Tilemap
{
public:
    Tilemap(int width, int height) : width(width), height(height)
    {
        tiles.resize(width * height);
    }

    Tile& getTile(int x, int y)
    {
        return tiles[y * width + x];
    }

    int getWidth() const
    {
        return width;
    }

    int getHeight() const
    {
        return height;
    }

    void RandomLevel()
    {
        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                Tile& tile = getTile(x, y);
                tile.type = (rand() % 5 == 0) ? TileType::Wall : TileType::Floor;
            }
        }
    }

    void WallsAsNonTraversable()
    {
        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                Tile& tile = getTile(x, y);
                if (tile.type == TileType::Wall)
                {
                    tile.adjacencyList.clear(); // Walls have no adjacency
                }
            }
        }
    }

    void createAdjacencyList()
    {
        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                Tile& tile = getTile(x, y);
                tile.adjacencyList.clear();

                if (tile.type == TileType::Floor)
                {
                    if (y > 0 && getTile(x, y - 1).type == TileType::Floor)
                        tile.adjacencyList.push_back((y - 1) * width + x);

                    if (y < height - 1 && getTile(x, y + 1).type == TileType::Floor)
                        tile.adjacencyList.push_back((y + 1) * width + x);

                    if (x > 0 && getTile(x - 1, y).type == TileType::Floor)
                        tile.adjacencyList.push_back(y * width + (x - 1));

                    if (x < width - 1 && getTile(x + 1, y).type == TileType::Floor)
                        tile.adjacencyList.push_back(y * width + (x + 1));
                }
            }
        }
    }

    bool isTileTraversable(int x, int y)
    {
        if (x < 0 || x >= width || y < 0 || y >= height)
            return false;

        return getTile(x, y).type == TileType::Floor;
    }

    void draw()
    {
        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                Tile& tile = getTile(x, y);
                Rectangle rect{ x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE };

                switch (tile.type)
                {
                case TileType::Floor:
                    DrawRectangleRec(rect, WHITE);
                    break;

                case TileType::Wall:
                    DrawRectangleRec(rect, GRAY);
                    break;
                }

                DrawRectangleLinesEx(rect, 1, BLACK);

                // Draw adjacency circles and lines
                if (tile.type == TileType::Floor)
                {
                    Vector2 center = { (rect.x + rect.width / 2), (rect.y + rect.height / 2) };
                    DrawCircle(center.x, center.y, 5, GREEN);

                    for (int adjTileIndex : tile.adjacencyList)
                    {
                        Tile& adjTile = tiles[adjTileIndex];
                        Rectangle adjRect{ adjTileIndex % width * TILE_SIZE, adjTileIndex / width * TILE_SIZE, TILE_SIZE, TILE_SIZE };
                        Vector2 adjCenter = { (adjRect.x + adjRect.width / 2), (adjRect.y + adjRect.height / 2) };
                        DrawLineEx(center, adjCenter, 1, GREEN);
                    }
                }
            }
        }
    }

private:
    int width;
    int height;
    std::vector<Tile> tiles;
};
int main()
{
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Tilemap_Lab 4");

    Tilemap tilemap(20, 15);
    tilemap.RandomLevel();
    tilemap.WallsAsNonTraversable();
    tilemap.createAdjacencyList();

    Vector2 characterPosition{ 0, 1 };

    SetTargetFPS(10);

    while (!WindowShouldClose())
    {
        // Handle input
        Vector2 newPosition = characterPosition;

        if (IsKeyDown(KEY_W) && tilemap.isTileTraversable(characterPosition.x, characterPosition.y - 1))
            newPosition.y--;
        if (IsKeyDown(KEY_S) && tilemap.isTileTraversable(characterPosition.x, characterPosition.y + 1))
            newPosition.y++;
        if (IsKeyDown(KEY_A) && tilemap.isTileTraversable(characterPosition.x - 1, characterPosition.y))
            newPosition.x--;
        if (IsKeyDown(KEY_D) && tilemap.isTileTraversable(characterPosition.x + 1, characterPosition.y))
            newPosition.x++;

        if (newPosition.x != characterPosition.x || newPosition.y != characterPosition.y)
        {
            characterPosition = newPosition;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        tilemap.draw();

        // Draw character sprite
        Rectangle characterRect{ characterPosition.x * TILE_SIZE, characterPosition.y * TILE_SIZE, TILE_SIZE, TILE_SIZE };
        DrawRectangleRec(characterRect, DARKBLUE);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}