#pragma once


#ifndef RW_SPRITE_H
#define RW_SPIRTE_H

#include <string>
#include "olcPixelGameEngine/olcPixelGameEngine.h"

class Sprite
{
public:
	Sprite(std::string filename)
	{
		sprite = new olc::Sprite(filename);
		decal = new olc::Decal(sprite);
	}

	olc::Sprite* getSprite()
	{
		return sprite;
	}
	
	void setOffset(float off)
	{
		offset = { off,off };
	}

	void setOffset(olc::vf2d off)
	{
		offset = off;
	}

	void render(olc::PixelGameEngine* game, olc::vf2d pos, float rot = 0.0f, olc::vf2d scl = { 1.0f,1.0f }, olc::Pixel tint = olc::WHITE)
	{
		game->DrawRotatedDecal( pos, decal, rot, offset, scl, tint );
	}

private:
	olc::Sprite* sprite;
	olc::Decal* decal;
	olc::vf2d offset;
};


class SpriteManager
{
public:
	SpriteManager(olc::PixelGameEngine* game)
	{
		this->game = game;
	}

	void load(std::string spriteName, std::string filename)
	{
		sprites.emplace(spriteName, new Sprite(filename));
	}

	Sprite* get(std::string spriteName)
	{
		return sprites.find(spriteName)->second;
	}

	void render(std::string spriteName, olc::vf2d pos, float rot = 0.0f, olc::vf2d scl = { 1.0f,1.0f }, olc::Pixel tint = olc::WHITE)
	{
		get(spriteName)->render(game, pos, rot, scl, tint);
	}


private:
	olc::PixelGameEngine* game;
	std::map<std::string, Sprite*> sprites;
};

#endif