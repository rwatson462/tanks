#include <cmath>
#include "map.h"
#include "particle.h"
#include "olcPixelGameEngine/olcPixelGameEngine.h"

constexpr int PROJECTILE_BULLET = 1;
constexpr int PROJECTILE_BULLET_AP = 2;
constexpr int PROJECTILE_BULLET_SPREAD = 3;
constexpr int PROJECTILE_MISSILE = 4;
constexpr int PROJECTILE_LANDMINE = 5;

class Projectile
{
public:
	float x, y;
	bool alive = true;
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

	void update(float fElapsedTime, TileMap* map, ParticleEmitter* particleEmitter, bool& collided)
	{
		collided = false;
		float diffX = fElapsedTime * sSinA;
		float diffY = fElapsedTime * sCosA;
		float newX = x + diffX;
		float newY = y - diffY;

		// check if projectile is off screen
		if (
			x < 0 || x >= map->mapWidth * map->f_tileSize
			||
			y < 0 || y > map->mapHeight * map->f_tileSize
			)
		{
			alive = false;
			return;
		}

		if (map->getObstacleTile(floor(x / map->f_tileSize), floor(y / map->f_tileSize)) != L' ')
		{
			// we're colliding with a wall, mark ourselves as no longer alive
			alive = false;
			// and create a fancy explosion animation with particles
			switch (type)
			{
			case PROJECTILE_BULLET:
				particleEmitter->create(x, y, rand() % 5, 1, 0.5f, olc::YELLOW);
				break;
			case PROJECTILE_BULLET_AP:
				particleEmitter->create(x, y, rand() % 15, 2, 0.5f, olc::RED);
				break;
			}

			// setting collided so the calling function can deal with the other part of the collision
			// i.e. the thing we collided with
			collided = true;
			return;
		}

		x += diffX;
		y -= diffY;
	};

protected:
	float speed;
	float sSinA, sCosA;
	float damage;
};
