#pragma once

#ifndef RW_TANK_H
#define RW_TANK_H

class Tank
{
public:
    float x = 0.0f, y = 0.0f, a = 0.0f;
    float speed = 0.0f, turnSpeed = 0.0f;
    olc::Pixel tint;
    int currentProjectile = 0;

    void setAngle(float newA)
    {
        a = newA;
        sinA = sin(a);
        cosA = cos(a);
    }

    void update(float fElapsedTime, olc::PixelGameEngine* game, TileMap* map);

private:
    // cached values that we'll only update when the tank rotates
    float sinA = 0.0f, cosA = 0.0f;

    void rotate(float fElapsedTime, int direction);
    void move(float fElapsedTime, int direction, olc::PixelGameEngine* game, TileMap* map);
};

#endif