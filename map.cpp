
#include "map.h"


void loadMap(std::wstring& map, std::string mapName)
{
    // todo attempt to load map file located at "./assets/maps/{mapName}"
    
    // small map
    map =  L"####################";
    map += L"#..................#";
    map += L"#..................#";
    map += L"#..................#";
    map += L"#..................#";
    map += L"#..................#";
    map += L"#..................#";
    map += L"#..................#";
    map += L"#..................#";
    map += L"#..................#";
    map += L"#..................#";
    map += L"#..................#";
    map += L"#..................#";
    map += L"#..................#";
    map += L"####################";
}

// todo change this so it builds a lit of obstacles and refactor it into loadMap above
void loadMapObstacles(std::wstring& obsMap, std::string mapName)
{
    obsMap =  L"                    ";
    obsMap += L"                    ";
    obsMap += L"   oo xx oo xx oo   ";
    obsMap += L"   o            o   ";
    obsMap += L"   o   xxxxxx   o   ";
    obsMap += L"   o            o   ";
    obsMap += L"   o   xxxxxx   o   ";
    obsMap += L"   o            o   ";
    obsMap += L"   oo xx oo xx oo   ";
    obsMap += L"                    ";
    obsMap += L"                    ";
    obsMap += L"                    ";
    obsMap += L"  oooooooooooooooo  ";
    obsMap += L"                    ";
    obsMap += L"                    ";
}