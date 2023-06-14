//#include "Tilemap.h"
//#include <iostream>
//
//Tilemap::Tilemap() : width(MAP_WIDTH), height(MAP_HEIGHT), tileSizeX(100), tileSizeY(100)
//{
//}
//
//void Tilemap::Initialize()
//{
//    tiles.resize(width);
//    for (int i = 0; i < width; i++)
//    {
//        tiles[i].resize(height, Tile::Floor);
//    }
//}
//
//void Tilemap::Draw()
//{
//    for (int y = 0; y < height; y++)
//    {
//        for (int x = 0; x < width; x++)
//        {
//            // Draw each tile
//            switch (tiles[x][y])
//            {
//            case Tile::Floor:
//                std::cout << "F ";
//                break;
//            case Tile::Wall:
//                std::cout << "W ";
//                break;
//            default:
//                break;
//            }
//        }
//        std::cout << std::endl;
//    }
//}
//
//void Tilemap::GenerateLevel()
//{
//    for (int y = 0; y < height; y++)
//    {
//        for (int x = 0; x < width; x++)
//        {
//            // Generate random tile types
//            if (rand() % 5 == 0)
//            {
//                tiles[x][y] = Tile::Wall;
//            }
//            else
//            {
//                tiles[x][y] = Tile::Floor;
//            }
//        }
//    }
//}
//
//bool Tilemap::IsTraversable(const TileCoord& tileCoord)
//{
//    if (tileCoord.x >= 0 && tileCoord.x < width && tileCoord.y >= 0 && tileCoord.y < height)
//    {
//        return tiles[tileCoord.x][tileCoord.y] == Tile::Floor;
//    }
//    return false;
//}
//
//void Tilemap::QueryAdjacency(const TileCoord& tileCoord)
//{
//    // Query for adjacent traversable tiles and perform actions
//    // based on the adjacency information.
//}
//
