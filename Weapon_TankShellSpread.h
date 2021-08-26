#pragma once


#ifndef RW_WEAPON_TANKSHELLSPREAD_H
#define RW_WEAPON_TANKSHELLSPREAD_H

#include "projectile.h"

class Weapon_TankShellSpread : public Projectile
{
public:
    static constexpr float reloadSpeed = 0.75f;
    static constexpr int type = 3;
};


#endif