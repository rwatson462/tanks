#pragma once


#ifndef RW_WEAPON_MISSILE_H
#define RW_WEAPON_MISSILE_H

#include "projectile.h"

class Weapon_Missile : public Projectile
{
public:
    static constexpr float reloadSpeed = 2.0f;
    static constexpr int type = 4;

    Weapon_Missile(float x, float y, float a)
    {
        m_x = x;
        m_y = y;

        // speed == 100
        m_sinA = sin(a) * 100.0f;
        m_cosA = cos(a) * 100.0f;

        m_dmg = 30.0f;
    }

    // todo override update() to enable the missile to "seek".  Required enemies first

    void render(SpriteManager* spriteManager)
    {
        // todo add missile sprite that faces it's angle
        spriteManager->render("bullet", { m_x, m_y }, 0.0f, { 1.5f, 1.5f }, olc::BLUE);
    }
private:
    void createParticles(ParticleEmitter* particleEmitter)
    {
        particleEmitter->create((int)m_x, (int)m_y, rand() % 25 + 25, 2, 1.0f, olc::BLUE);
    }
};


#endif