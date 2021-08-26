#pragma once


#ifndef RW_WEAPON_MISSILE_H
#define RW_WEAPON_MISSILE_H

#include "projectile.h"

class Weapon_Missile : public Projectile
{
public:
    static constexpr float reloadSpeed = 2.0f;
    static constexpr int type = 4;
};


#endif