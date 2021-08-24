#pragma once

#ifndef RW_PARTICLE_H
#define RW_PARTICLE_H

#include "olcPixelGameEngine/olcPixelGameEngine.h"


class Particle
{
public:
	float x = 0;
	float y = 0;
	float life = 0.0f;
	olc::Pixel colour;
	bool alive = true;

	Particle(int x, int y, float life, olc::Pixel colour)
	{
		this->x = (float)x;
		this->y = (float)y;
		this->life = life;
		this->colour = colour;
	}

	void update(float fElapsedTime)
	{
		this->life -= fElapsedTime;
		if (this->life <= 0)
		{
			this->alive = false;
		}
	}

	void render(olc::PixelGameEngine* game)
	{
		game->FillRectDecal(
			{ x,y },
			{ 1.0f, 1.0f },
			colour
		);
	}
};


class ParticleEmitter
{
public:

	void create(int x, int y, float life = 1.0f, olc::Pixel colour = olc::WHITE)
	{
		particles.push_back(new Particle(x, y, life, colour));
	}

	void update(float fElapsedTime)
	{
		for (Particle* p : particles)
		{
			p->update(fElapsedTime);
		}

		// loop over particles, removing dead ones
		if (!particles.empty())
		{
			// not 100% sure what idx is, possibly an integer index that we can use to break the chain in the list
			// remove_it sorts the particles with all that are alive first, followed by all that are not alive
			// then the first not-alive particle is removed which means all other not-alive particles are also
			// deleted
			auto idx = remove_if(particles.begin(), particles.end(), [&](Particle* p) {
				return !p->alive;
				});
			if (idx != particles.end())
			{
				particles.erase(idx);
			}
		}
	}

	void render(olc::PixelGameEngine* game)
	{
		for (Particle* p : particles)
		{
			p->render(game);
		}
	}

private:
	std::vector<Particle*> particles;
};

#endif