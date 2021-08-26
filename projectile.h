
#ifndef RW_PROJECTILE_H
#define RW_PROJECTILE_H

#include <cmath>
#include "map.h"
#include "sprite.h"
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
    // deliberately named these without m_ as it'll be accessed from public
	bool alive = true;
    int type = 0;

    float m_x = 0.0f, m_y = 0.0f;

	void update(float fElapsedTime, TileMap* map, ParticleEmitter* particleEmitter, bool& collided)
	{
		collided = false;
		float diffX = fElapsedTime * m_sinA;
		float diffY = fElapsedTime * m_cosA;
		float newX = m_y + diffX;
		float newY = m_y - diffY;

		// check if projectile is off screen
		if (
            m_x < 0 || m_x >= map->mapWidth * map->f_tileSize
			||
            m_y < 0 || m_y > map->mapHeight * map->f_tileSize
        )
		{
			alive = false;
			return;
		}

		if (map->getObstacleTile(floor(m_x / map->f_tileSize), floor(m_y / map->f_tileSize)) != L' ')
		{
			// we're colliding with a wall, mark ourselves as no longer alive
			alive = false;

			// and create a fancy explosion animation with particles
			createParticles(particleEmitter);

//			switch (type)
//			{
//			case PROJECTILE_BULLET:
//                particleEmitter->create((int)m_x, (int)m_y, rand() % 5, 1, 0.5f, olc::YELLOW);
//				break;
//			case PROJECTILE_BULLET_AP:
//				particleEmitter->create((int)m_x, (int)m_y, rand() % 15, 2, 0.5f, olc::RED);
//				break;
//			}

			// setting collided so the calling function can deal with the other part of the collision
			// i.e. the thing we collided with
			collided = true;
			return;
		}

        m_x += diffX;
        m_y -= diffY;
	};

	virtual void render(SpriteManager* spriteManager) {}

protected:
	float m_sinA = 0.0f, m_cosA = 0.0f;
	float m_dmg = 0.0f;

	virtual void createParticles(ParticleEmitter* particleEmitter) {}
};



class TankShell : public Projectile
{
public:
    static const float reloadSpeed;

    TankShell(float x, float y, float a)
    {
        m_x = x;
        m_y = y;

        // speed == 200
        m_sinA = sin(a) * 200.0f;
        m_cosA = cos(a) * 200.0f;

        m_dmg = 1.0f;
        type = PROJECTILE_BULLET;
    }

    void render(SpriteManager* spriteManager) override
    {
        spriteManager->render("bullet", { m_x, m_y });
    }

private:
    void createParticles( ParticleEmitter* particleEmitter) override
    {
        particleEmitter->create((int)m_x, (int)m_y, rand() % 5, 1, 0.5f, olc::YELLOW);
    }
};


class TankShellAP : public Projectile
{
public:
    static const float reloadSpeed;

    TankShellAP(float x, float y, float a)
    {
        m_x = x;
        m_y = y;

        // speed == 150
        m_sinA = sin(a) * 150.0f;
        m_cosA = cos(a) * 150.0f;

        m_dmg = 5.0f;
        type = PROJECTILE_BULLET_AP;
    }

    void render( SpriteManager* spriteManager) override
    {
        spriteManager->render("bullet", { m_x, m_y }, 0.0f, { 1.0f, 1.0f }, olc::RED);
    }

private:
    void createParticles( ParticleEmitter* particleEmitter) override
    {
        particleEmitter->create((int)m_x, (int)m_y, rand() % 15, 2, 0.5f, olc::RED);
    }
};


class TankShellSpread : public Projectile
{
public:
    static const float reloadSpeed;
};

class Missile : public Projectile
{
public:
    static const float reloadSpeed;
};

class LandMine : public Projectile
{
public:
    static const float reloadSpeed;
};



#endif