#include <cmath>

constexpr int PROJECTILE_BULLET = 1;
constexpr int PROJECTILE_BULLET_AP = 2;
constexpr int PROJECTILE_MISSILE = 3;
constexpr int PROJECTILE_LANDMINE = 4;

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
