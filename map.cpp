
#include "map.h"


int TileMap::getTileId(int tileX, int tileY)
{
    return tileY * this->mapWidth + tileX;
}

void TileMap::alterObstacleTile(int tileX, int tileY, wchar_t newValue)
{
    obsMap[ getTileId(tileX,tileY) ] = newValue;
}

wchar_t TileMap::getMapTile(int tileX, int tileY)
{
    return this->map[getTileId(tileX,tileY)];
}

wchar_t TileMap::getObstacleTile(int tileX, int tileY)
{
    return this->obsMap[getTileId(tileX,tileY)];
}

void TileMap::loadMap(std::string mapName)
{
    // small map
    this->map =  L"....................";
    this->map += L"....................";
    this->map += L"....................";
    this->map += L"....................";
    this->map += L"....................";
    this->map += L"....................";
    this->map += L"....................";
    this->map += L"....................";
    this->map += L"....................";
    this->map += L"....................";
    this->map += L"....................";
    this->map += L"....................";
    this->map += L"####################";
    this->map += L"####################";
    this->map += L"####################";
}

void TileMap::loadMapObstacles(std::string mapName)
{
    // todo change this so it builds a lit of obstacles and refactor it into loadMap above
    this->obsMap =  L"oooooooooooooooooooo";
    this->obsMap += L"o                  o";
    this->obsMap += L"o    o x  x  x     o";
    this->obsMap += L"o    x    o        o";
    this->obsMap += L"o    x          x  o";
    this->obsMap += L"oxxxxxxxxxxxxxxxxx o";
    this->obsMap += L"o        x         o";
    this->obsMap += L"o      o     x     o";
    this->obsMap += L"o                  o";
    this->obsMap += L"o    ooo oo ooo    o";
    this->obsMap += L"o                  o";
    this->obsMap += L"o oooo oooooo oooo o";
    this->obsMap += L"o                  o";
    this->obsMap += L"o                  o";
    this->obsMap += L"oooooo        oooooo";
}


void TileMap::load(std::string mapName)
{
    // todo attempt to load map file located at "./assets/maps/{mapName}"
    // determine number of tiles for map
    mapWidth = 20;
    mapHeight = 15;
    loadMap(mapName);
    loadMapObstacles(mapName);
}