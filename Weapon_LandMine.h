#pragma once


#ifndef RW_WEAPON_LANDMINE_H
#define RW_WEAPON_LANDMINE_H

#include "projectile.h"

class Weapon_LandMine : public Projectile
{
public:
    static constexpr float reloadSpeed = 5.0f;
    static constexpr int type = 5;
};


#endif