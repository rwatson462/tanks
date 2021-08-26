#pragma once


#ifndef RW_WEAPON_TANKSHELL_H
#define RW_WEAPON_TANKSHELL_H

#include "projectile.h"

class Weapon_TankShell : public Projectile
{
public:
    static constexpr float reloadSpeed = 0.2f;
    static constexpr int type = 1;

    Weapon_TankShell(float x, float y, float a)
    {
        m_x = x;
        m_y = y;

        // speed == 200
        m_sinA = sin(a) * 200.0f;
        m_cosA = cos(a) * 200.0f;

        m_dmg = 1.0f;
    }

    void render(SpriteManager* spriteManager)
    {
        spriteManager->render("bullet", { m_x, m_y });
    }
private:
    void createParticles(ParticleEmitter* particleEmitter)
    {
        particleEmitter->create((int)m_x, (int)m_y, rand() % 5, 1, 0.5f, olc::YELLOW);
    }
};


#endif