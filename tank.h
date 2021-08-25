#pragma once

#ifndef RW_TANK_H
#define RW_TANK_H

#include "sprite.h"

class Tank
{
public:

    // screen coordinates
    float x = 0.0f;
    float y = 0.0f;

    // direction of travel in radians
    float a = 0.0f;
    // aiming of turret
    float d = 0.0f;

    // maximum speed
    float speed = 0.0f;

    // speed of rotating
    float turnSpeed = 0.0f;

    // apply a nice colour tint to the vehicle
    olc::Pixel tint;

    // allow this vehicle to use multiple weapons
    int currentProjectile = 0;

    // world size distance between centre of player and edge for a bounding box
    float collisionSize;

    // the player must wait for this long until they can fire again
    float maxReloadTime = 2.0f;
    // a counter used to tell us if we can fire again
    float reloadTime = 0.0f;

    bool canShoot()
    {
        return reloadTime == 0.0f;
    }

    // extracted to a function so we can recalculated some cached values
    void setAngle(float newA)
    {
        a = newA;
        sinA = sin(a);
        cosA = cos(a);
    }

    void update(float fElapsedTime, olc::PixelGameEngine* game, TileMap* map)
    {
        if (game->GetKey(olc::W).bHeld)
        {
            move(fElapsedTime, 1, map);
        }
        if (game->GetKey(olc::S).bHeld)
        {
            move(fElapsedTime, -1, map);
        }
        if (game->GetKey(olc::A).bHeld)
        {
            rotate(fElapsedTime, -1);
        }
        if (game->GetKey(olc::D).bHeld)
        {
            rotate(fElapsedTime, 1);
        }

        rotateTurret(fElapsedTime, game);

        if (reloadTime >= 0.0f) reloadTime -= fElapsedTime;
        if (reloadTime <= 0.0f) reloadTime = 0.0f;
    }

    void render(SpriteManager* spriteManager)
    {
        // draw the tank chassis
        spriteManager->render(
            "tankChassis",
            { x, y },
            a,
            { 1.0f, 1.0f },
            tint
        );

        // draw the turret
        spriteManager->render(
            "tankTurret",
            { x, y },
            d,
            { 1.0f, 1.0f },
            tint
        );
    }

private:
    // cached values that we'll only update when the tank rotates
    float sinA = 0.0f;
    float cosA = 0.0f;

    void checkCollision(TileMap* map, float& diffX, float& diffY)
    {
        float newX = x + diffX;
        float newY = y + diffY;

        // check for see if an obstacle exists in the tile we'd be in if we applied the x/yDiff

        int newX_tile, newY_tile;

        // check edges, not corners

        if (newX < x)
        {
            // we're moving west
            newX_tile = (int)((newX - collisionSize) / map->f_tileSize);
            newY_tile = (int)(newY / map->f_tileSize);
            if (map->getObstacleTile(newX_tile, newY_tile) != L' ')
            {
                diffX = 0;
            }

        }
        else if (newX > x)
        {
            // we're moving east
            newX_tile = (int)((newX + collisionSize) / map->f_tileSize);
            newY_tile = (int)(newY / map->f_tileSize);
            if (map->getObstacleTile(newX_tile, newY_tile) != L' ')
            {
                diffX = 0;
            }
        }

        if (newY < y)
        {
            // we're moving north
            newX_tile = (int)(newX / map->f_tileSize);
            newY_tile = (int)((newY + collisionSize) / map->f_tileSize);
            if (map->getObstacleTile(newX_tile, newY_tile) != L' ')
            {
                diffY = 0;
            }

        }
        else if (newY > y)
        {
            // we're moving south
            newX_tile = (int)(newX / map->f_tileSize);
            newY_tile = (int)((newY - collisionSize) / map->f_tileSize);
            if (map->getObstacleTile(newX_tile, newY_tile) != L' ')
            {
                diffY = 0;
            }
        }

    }
    void rotate(float fElapsedTime, int direction)
    {
        // if direction > 0, we turn clockwise
        if (direction > 0)
        {
            float newA = a + fElapsedTime * turnSpeed;
            if (newA > 2 * M_PI) newA -= 2 * M_PI;
            setAngle(newA);
            return;
        }

        // if direction < 0, we turn anti-clockwise
        if (direction < 0)
        {
            float newA = a - fElapsedTime * turnSpeed;
            if (newA < 0) newA += 2 * M_PI;
            setAngle(newA);
            return;
        }
    }
    void rotateTurret(float fElapsedTime, olc::PixelGameEngine* game)
    {
        // todo: make turret move towards the mouse but at a slower speed

        // calculate x/y distances from player to mouse
        float xD = game->GetMouseX() - (float)x;
        float yD = -(game->GetMouseY() - (float)y);

        // handle possible divide by zero errors
        if (yD == 0)
        {
            if (xD > 0)
            {
                d = M_PI * 0.5;
            }
            else if (xD < 0)
            {
                d = M_PI * 1.5;
            }
            else
            {
                d = 0;
            }
        }
        else
        {
            d = atan(xD / yD);
            if (yD < 0) d += M_PI;
        }
    }
    void move(float fElapsedTime, int direction, TileMap* map)
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
            checkCollision(map, diffX, diffY);

            // we're allowed to move
            x += diffX;
            y -= diffY;
            return;
        }

        // if direction < 0, we move backwards
        if (direction < 0)
        {
            float diffX = fElapsedTime * speed * sinA;
            float diffY = fElapsedTime * speed * cosA;
            float newX = x - diffX;
            float newY = y + diffY;

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
            checkCollision(map, diffX, diffY);

            // we're allowed to move
            x -= diffX;
            y += diffY;
            return;
        }
    }
};

#endif