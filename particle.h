#pragma once

#ifndef RW_PARTICLE_H
#define RW_PARTICLE_H

#include "olcPixelGameEngine/olcPixelGameEngine.h"



constexpr int _MAX_PARTICLE_COUNT = 1000;

class Particle
{
public:
	bool alive = true;

	Particle(int x, int y, float s, float life, float angle, olc::Pixel colour)
	{
		// starting position of particle
		this->x = (float)x;
		this->y = (float)y;

		// size in pixels when drawing the particle
		this->s = s;

		// time in seconds that this particle will live for
		this->life = this->lifeTimeMax = life;

		// colour of the particle
		this->colour = colour;

		// preconfigure the x/y changes to this particle
		// 4.0 is an arbitrary speed multiplier
		xOffset = sin(angle) / 4.0f;
		yOffset = cos(angle) / 4.0f;
	}

	void update(float fElapsedTime)
	{
		// reduce remaining lifetime of the particle and mark is as dead when appropriate
		this->life -= fElapsedTime;
		if (this->life <= 0)
		{
			this->alive = false;
			return;
		}

		// move the particle by an arbitrary amount
		x += xOffset;
		y += yOffset;

		// fade out as we get towards death
		colour.a = life / lifeTimeMax * 255;
	}

	void render(olc::PixelGameEngine* game)
	{
		game->FillRectDecal(
			{ x,y },
			{ s,s },
			colour
		);
	}

private:
	float x = 0.0f;
	float y = 0.0f;
	float xOffset = 0.0f;
	float yOffset = 0.0f;
	float s = 1.0f;
	float life = 0.0f;
	float lifeTimeMax = 0.0f;
	olc::Pixel colour;
};


class ParticleEmitter
{
public:

	void create(int x, int y, int count = 1, int size = 1, float max_life = 1.0f, olc::Pixel colour = olc::WHITE)
	{
	    float life, angle;
		int index;

	    for( int i = 0; i <= count; i++ ) {

			index = -1;

			for (int j = 0; j < _MAX_PARTICLE_COUNT; j++)
			{
				if (particleList[j] == nullptr)
				{
					index = j;
					break;
				}
			}

			if (index == -1)
			{
				printf("All particle slots in use!\n");
				return;
			}

			particleList[index] = new Particle(
				x, y, size,
				(float)(rand() / (RAND_MAX / max_life)),
				(float)(rand() / (RAND_MAX / (M_PI * 2))),
				colour
			);
        }
	}

	void update(float fElapsedTime)
	{
		for( int i = 0; i < _MAX_PARTICLE_COUNT; i++)
		{
			if (particleList[i] == nullptr) continue;

			particleList[i]->update(fElapsedTime);
			if (!particleList[i]->alive) particleList[i] = nullptr;
		}
	}

	void render(olc::PixelGameEngine* game)
	{
		for( int i = 0; i < _MAX_PARTICLE_COUNT; i++)
		{
			if (particleList[i] == nullptr) continue;
			particleList[i]->render(game);
		}
	}

private:
	Particle* particleList[_MAX_PARTICLE_COUNT] = { nullptr };
};

#endif