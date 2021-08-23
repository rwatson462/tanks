
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
    this->map =  L"####################";
    this->map += L"#..................#";
    this->map += L"#..................#";
    this->map += L"#..................#";
    this->map += L"#..................#";
    this->map += L"#..................#";
    this->map += L"#..................#";
    this->map += L"#..................#";
    this->map += L"#..................#";
    this->map += L"#..................#";
    this->map += L"#..................#";
    this->map += L"#..................#";
    this->map += L"#..................#";
    this->map += L"#..................#";
    this->map += L"####################";
}

void TileMap::loadMapObstacles(std::string mapName)
{
    // todo change this so it builds a lit of obstacles and refactor it into loadMap above
    this->obsMap =  L"                    ";
    this->obsMap += L"                    ";
    this->obsMap += L"   oo xx oo xx oo   ";
    this->obsMap += L"   o            o   ";
    this->obsMap += L"   o   xxxxxx   o   ";
    this->obsMap += L"   o            o   ";
    this->obsMap += L"   o   xxxxxx   o   ";
    this->obsMap += L"   o            o   ";
    this->obsMap += L"   oo xx oo xx oo   ";
    this->obsMap += L"                    ";
    this->obsMap += L"                    ";
    this->obsMap += L"                    ";
    this->obsMap += L"  oooooooooooooooo  ";
    this->obsMap += L"                    ";
    this->obsMap += L"                    ";
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