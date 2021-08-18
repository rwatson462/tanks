#pragma once
#include <math.h>

class Projectile
{
public:
	float x, y;
	bool isAlive = true;
	int projectileType = 0;

	Projectile(float startX, float startY, float angle, int type)
	{
		projectileType = type;
		x = startX;
		y = startY;
		sSinA = speed * sin(angle);
		sCosA = speed * cos(angle);
	}

	void update(float fElapsedTime);


protected:
	float sSinA, sCosA;

	// this will be overwritten by subclasses to control different speeds of different projectiles
	float speed = 0.0f;
};



class Bullet: Projectile
{
private:
	// bullets are fast
	float speed = 200.0f;

	// but deal little damage
	float damage = 0.2f;
};
