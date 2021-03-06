#pragma once


#ifndef RW_WEAPON_MISSILE_H
#define RW_WEAPON_MISSILE_H

#include "projectile.h"

class Weapon_Missile : public Projectile
{
public:
    static constexpr float reloadSpeed = 2.0f;
    static constexpr int type = 4;

    Weapon_Missile(olc::PixelGameEngine* game, float x, float y, float a)
    {
        m_x = x;
        m_y = y;

        // missile sprite is 90 degree rotated
        m_a = a - M_PI/2;

        // speed == 100
        m_sinA = sin(a) * 100.0f;
        m_cosA = cos(a) * 100.0f;

        m_dmg = 30.0f;

        // also store mouse-coords of click to mark as missile target location
        m_mouseX = game->GetMouseX();
        m_mouseY = game->GetMouseY();
    }

    // todo override update() to enable the missile to "seek".  Required enemies first

    void render(SpriteManager* spriteManager)
    {
        // todo add missile sprite that faces it's angle
        spriteManager->render("missile", { m_x, m_y }, m_a);

        spriteManager->render("x", { m_mouseX, m_mouseY }, 0.0f, { 1.0f,1.0f }, olc::Pixel(255, 0, 0, 128));
    }
private:
    float m_a = 0.0f;
    float m_mouseX = 0.0f;
    float m_mouseY = 0.0f;

    void createParticles(ParticleEmitter* particleEmitter)
    {
        particleEmitter->create((int)m_x, (int)m_y, rand() % 25 + 25, 2, 1.0f, olc::BLUE);
    }
};


#endif