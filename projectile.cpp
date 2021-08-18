#include <iostream>
#include <math.h>

/**
	Yes, yes, having multiple classes in the same .cpp file is probably
	an anti-pattern but I don't care.  Maybe one day I'll learn how to
	write proper c++ and come back and fix this.  Or maybe I won't.
*/

class Projectile
{
public:
	float x, y;
	bool isAlive = true;
	int type;

	Projectile(int projectileType, float startX, float startY, float angle, float speed, float damage )
	{
		type = projectileType;
		x = startX;
		y = startY;
		sSinA = speed * sin(angle);
		sCosA = speed * cos(angle);

		// todo extract this to some enum
		this->speed = speed;
		this->damage = damage;
	}

	void update(float fElapsedTime)
	{
		x += fElapsedTime * sSinA;
		y -= fElapsedTime * sCosA;
	};

protected:
	float speed;
	float sSinA, sCosA;
	float damage;
};
