#pragma once

#ifndef RW_MAP_H
#define RW_MAP_H

#include <string>

class TileMap
{
public:
    int mapWidth, mapHeight;
    float f_tileSize, f_halfTileSize;

    void load(std::string mapName);
    wchar_t getMapTile(int tileX, int tileY);
    wchar_t getObstacleTile(int tileX, int tileY);
    int getTileId(int tileX, int tileY);

    void alterObstacleTile(int tileX, int tileY, wchar_t newValue);

private:
    std::wstring map, obsMap;
    void loadMap(std::string mapName);
    void loadMapObstacles(std::string mapName);
};

#endif