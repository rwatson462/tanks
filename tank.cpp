/*
#define _USE_MATH_DEFINES
#include <cmath>
#include "tank.h"
#include "olcPixelGameEngine/olcPixelGameEngine.h"
#include "map.h"

//private
void Tank::setAngle(float newA)
{
    a = newA;
    sinA = sin(a);
    cosA = cos(a);
}

//private
void Tank::move(float fElapsedTime, int direction, olc::PixelGameEngine* game, TileMap* map)
{
    // if direction > 0, we move forwards
    if (direction > 0)
    {
        float diffX = fElapsedTime * speed * sinA;
        float diffY = fElapsedTime * speed * cosA;
        float newX = x + diffX;
        float newY = y - diffY;

        // check if the tank is leaving the world
        if (newX < map->f_halfTileSize || newX + map->f_halfTileSize > map->mapWidth * map->f_tileSize)
        {
            diffX = 0;
        }

        if (newY < map->f_halfTileSize || newY + map->f_halfTileSize > map->mapHeight * map->f_tileSize)
        {
            diffY = 0;
        }
        
        
        // diffX and diffY are passed by reference so will be updated if needed
        //game->checkCollision(diffX, diffY);

        // we're allowed to move
        x += diffX;
        y -= diffY;
        return;
    }

    // if direction < 0, we move backwards
    if (direction < 0)
    {
        return;
    }
}

//private
void Tank::rotate(float fElapsedTime, int direction)
{
    // if direction > 0, we turn clockwise
    if (direction > 0)
    {
        float newA = fElapsedTime * turnSpeed;
        if (newA > 2 * M_PI) newA -= 2 * M_PI;
        setAngle(newA);
        return;
    }

    // if direction < 0, we turn anti-clockwise
    if (direction < 0)
    {
        float newA = -fElapsedTime * turnSpeed;
        if (newA < 0) newA += 2 * M_PI;
        setAngle(newA);
        return;
    }
}

// public
void Tank::update(float fElapsedTime, olc::PixelGameEngine* game, TileMap* map)
{
    if (game->GetKey(olc::W).bHeld)
    {
        move(fElapsedTime, 1, game, map);
    }
    if (game->GetKey(olc::S).bHeld)
    {
        move(fElapsedTime, -1, game, map);
    }
    if (game->GetKey(olc::A).bHeld)
    {
        rotate(fElapsedTime, -1);
    }
    if (game->GetKey(olc::D).bHeld)
    {
        rotate(fElapsedTime, 1);
    }
}
*/