#pragma once


#ifndef RW_WEAPON_TANKSHELLAP_H
#define RW_WEAPON_TANKSHELLAP_H

#include "projectile.h"

class Weapon_TankShellAP : public Projectile
{
public:
    static constexpr float reloadSpeed = 0.5f;
    static constexpr int type = 2;

    Weapon_TankShellAP(float x, float y, float a)
    {
        m_x = x;
        m_y = y;

        // speed == 150
        m_sinA = sin(a) * 150.0f;
        m_cosA = cos(a) * 150.0f;

        m_dmg = 5.0f;
    }

    void render(SpriteManager* spriteManager)
    {
        spriteManager->render("bullet", { m_x, m_y }, 0.0f, { 1.0f, 1.0f }, olc::RED);
    }
private:
    void createParticles(ParticleEmitter* particleEmitter)
    {
        particleEmitter->create((int)m_x, (int)m_y, rand() % 15, 2, 0.5f, olc::RED);
    }
};


#endif