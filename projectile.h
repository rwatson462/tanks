
#ifndef RW_PROJECTILE_H
#define RW_PROJECTILE_H

#include <cmath>
#include "map.h"
#include "sprite.h"
#include "particle.h"
#include "olcPixelGameEngine/olcPixelGameEngine.h"

class Projectile
{
public:
    // deliberately named these without m_ as it'll be accessed from public
	bool alive = true;
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
			// we're colliding with a wall
			alive = false;
            collided = true;
			createParticles(particleEmitter);
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
    static constexpr float reloadSpeed = 0.2f;
    static constexpr int type = 1;

    TankShell(float x, float y, float a)
    {
        m_x = x;
        m_y = y;

        // speed == 200
        m_sinA = sin(a) * 200.0f;
        m_cosA = cos(a) * 200.0f;

        m_dmg = 1.0f;
    }

    void render(SpriteManager* spriteManager)
    {
        spriteManager->render("bullet", { m_x, m_y });
    }
private:
    void createParticles(ParticleEmitter* particleEmitter)
    {
        particleEmitter->create((int)m_x, (int)m_y, rand() % 5, 1, 0.5f, olc::YELLOW);
    }
};

class TankShellAP : public Projectile
{
public:
    static constexpr float reloadSpeed = 0.5f;
    static constexpr int type = 2;

    TankShellAP(float x, float y, float a)
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

class TankShellSpread : public Projectile
{
public:
    static constexpr float reloadSpeed = 0.75f;
    static constexpr int type = 3;
};

class Missile : public Projectile
{
public:
    static constexpr float reloadSpeed = 2.0f;
    static constexpr int type = 4;
};

class LandMine : public Projectile
{
public:
    static constexpr float reloadSpeed = 5.0f;
    static constexpr int type = 5;
};



#endif