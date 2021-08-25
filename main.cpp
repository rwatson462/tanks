#define _USE_MATH_DEFINES
#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine/olcPixelGameEngine.h"
#include "map.h"
#include "tank.h"
#include "particle.h"
#include "projectile.cpp"
#include <cmath>


class TanksGame : public olc::PixelGameEngine
{
public:
    TanksGame()
    {
        sAppName = "Tanks!";
    }

private:

    bool isRunning = true;
    bool isPaused = false;

    olc::Sprite *grassSprite;
    olc::Sprite *dirtSprite;
    olc::Decal *grassDecal;
    olc::Decal *dirtDecal;

    olc::Sprite* destructableWallSprite;
    olc::Decal* destructableWallDecal;
    olc::Sprite* solidWallSprite;
    olc::Decal* solidWallDecal;

    olc::Sprite *tankChassisSprite;
    olc::Sprite *tankTurretSprite;
    olc::Decal *tankChassisDecal;
    olc::Decal *tankTurretDecal;
    olc::Sprite* tankTrackSprite;
    olc::Decal* tankTrackDecal;

    olc::Sprite *bulletSprite;
    olc::Decal* bulletDecal;
    float bulletDrawOffset;
    std::vector<Projectile*> projectiles;

    Tank* player;
    float playerA = 0.0f;

    std::vector<olc::vi2d> tilesOfInterest;
    std::vector<olc::vi2d> tracks;

    TileMap* map;
    ParticleEmitter* particleEmitter;

    bool OnUserCreate() override
    {
        map = new TileMap();
        map->load("start");
        map->f_tileSize = (float)ScreenHeight() / (float)map->mapHeight;
        map->f_halfTileSize = map->f_tileSize / 2.0f;

        particleEmitter = new ParticleEmitter();

        grassSprite = new olc::Sprite("./assets/grasstop.png");
        dirtSprite = new olc::Sprite("./assets/dirt.png");
        grassDecal = new olc::Decal( grassSprite );
        dirtDecal = new olc::Decal( dirtSprite );

        tankChassisSprite = new olc::Sprite( "./assets/tank-chassis.png" );
        tankTurretSprite = new olc::Sprite( "./assets/tank-turret.png" );
        tankChassisDecal = new olc::Decal( tankChassisSprite );
        tankTurretDecal = new olc::Decal(tankTurretSprite);
        tankTrackSprite = new olc::Sprite("./assets/tank-tracks.png");
        tankTrackDecal = new olc::Decal(tankTrackSprite);

        destructableWallSprite = new olc::Sprite("./assets/destructable-wall.png");
        destructableWallDecal = new olc::Decal(destructableWallSprite);
        solidWallSprite = new olc::Sprite("./assets/solid-wall.png");
        solidWallDecal = new olc::Decal(solidWallSprite);

        bulletSprite = new olc::Sprite("./assets/bullet.png");
        bulletDecal = new olc::Decal(bulletSprite);
        bulletDrawOffset = bulletSprite->width / 2.0f;

        player = new Tank();
        player->x = map->mapWidth * map->f_tileSize / 2.0f;
        player->y = map->mapHeight * map->f_tileSize - map->f_tileSize * 2.0f + map->f_halfTileSize;
        player->setAngle(0.0f);
        player->speed = 40.0f;
        player->turnSpeed = 2.0f;
        player->tint = olc::Pixel(255, 200, 255, 255);
        player->currentProjectile = PROJECTILE_BULLET;
        player->collisionSize = map->f_tileSize * 0.4f; // distance from centre of tank to edge of hit box

        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        Update( fElapsedTime );
        Render( fElapsedTime );
        return isRunning;
    }

    void Update( float fElapsedTime )
    {
        // quit with escape
        if (GetKey(olc::ESCAPE).bPressed)
        {
            isRunning = false;
            return;
        }

        // pause with 'p'
        if( GetKey(olc::P ).bPressed )
        {
            isPaused = !isPaused;
        }

        // take no further action if the game is paused
        if( isPaused )
        {
            return;
        }

        if (GetKey(olc::R).bPressed)
        {
            map->load("start");
        }

        player->update(fElapsedTime, this, map);
        changeWeapon();
        shoot();
        updateProjectiles(fElapsedTime);
        particleEmitter->update(fElapsedTime);
    }

    void changeWeapon()
    {
        if( !GetKey(olc::TAB).bPressed) return;

        switch(player->currentProjectile)
        {
            case PROJECTILE_BULLET:
                player->currentProjectile = PROJECTILE_BULLET_AP;
                break;
            case PROJECTILE_BULLET_AP:
                player->currentProjectile = PROJECTILE_BULLET_SPREAD;
                break;
            case PROJECTILE_BULLET_SPREAD:
                player->currentProjectile = PROJECTILE_BULLET;
                // no other weapons currently enabled
                break;
                player->currentProjectile = PROJECTILE_MISSILE;
                break;
            case PROJECTILE_MISSILE:
                player->currentProjectile = PROJECTILE_LANDMINE;
                break;
            case PROJECTILE_LANDMINE:
                player->currentProjectile = PROJECTILE_BULLET;
                break;
        }
    }

    void shoot()
    {
        // left mouse = bullet
        if (GetMouse( 0 ).bHeld || GetKey(olc::SPACE).bHeld)
        {
            // attempt to fire whatever the current projectile is
            if (!player->canShoot()) return;

            // adjust start position of bullet so it appears to come from the end of the turret
            float startX = player->x + 8.0f * sin(player->d);
            float startY = player->y - 8.0f * cos(player->d);

            // TODO create struct of all projectiles with their stats so we don't need to refer to all of them in here
            Projectile* b;

            float a1, a2;

            switch(player->currentProjectile)
            {
                case PROJECTILE_BULLET:
                    projectiles.push_back(new Projectile(PROJECTILE_BULLET, startX, startY, player->d, 200.0f, 0.2f));
                    break;
                case PROJECTILE_BULLET_AP:
                    projectiles.push_back(new Projectile(PROJECTILE_BULLET_AP, startX, startY, player->d, 150.0f, 2.0f));
                    break;
                case PROJECTILE_BULLET_SPREAD:
                    a1 = player->d - M_PI / 12;
                    a2 = player->d + M_PI / 12;
                    if (a1 < 0) a1 += M_PI * 2;
                    if (a2 > M_PI * 2) a2 -= M_PI * 2;
                    projectiles.push_back(new Projectile(PROJECTILE_BULLET, startX, startY, a1, 200.0f, 0.2f));
                    projectiles.push_back(new Projectile(PROJECTILE_BULLET, startX, startY, player->d, 200.0f, 0.2f));
                    projectiles.push_back(new Projectile(PROJECTILE_BULLET, startX, startY, a2, 200.0f, 0.2f));
                    break;
                case PROJECTILE_MISSILE:
                    break;
                    projectiles.push_back(new Projectile(PROJECTILE_MISSILE, startX, startY, player->d, 50.0f, 10.0f));
                    break;
                case PROJECTILE_LANDMINE:
                    break;
                    projectiles.push_back(new Projectile(PROJECTILE_LANDMINE, player->x, player->y, player->d, 0.0f, 10.0f));
                    break;
            }

            player->reloadTime = player->maxReloadTime;
        }
    }

    void updateProjectiles(float fElapsedTime)
    {
        bool collided;

        // move each bullet
        for (auto &projectile : projectiles)
        {
            projectile->update(fElapsedTime, map, particleEmitter, collided);

            if (!collided) continue;

            // convert projectile's x/y to map coords
            int projX = floor(projectile->x / map->f_tileSize);
            int projY = floor(projectile->y / map->f_tileSize);

            if (map->getObstacleTile(projX, projY) == L'x')
            {
                // we're colliding with a destructable wall, destruct it
                map->alterObstacleTile(projX, projY, L' ');

                // create some particles to show the block being destroyed
                float worldX = projX * map->f_tileSize;
                float worldY = projY * map->f_tileSize;
                particleEmitter->create(
                    worldX + map->f_halfTileSize/2, 
                    worldY + map->f_halfTileSize/2, 
                    rand() % 50, 4, 0.5f, olc::GREY
                );
                particleEmitter->create(
                    worldX + map->f_halfTileSize*1.5,
                    worldY + map->f_halfTileSize/2, 
                    rand() % 50, 4, 0.5f, olc::GREY
                );
                particleEmitter->create(
                    worldX + map->f_halfTileSize/2,
                    worldY + map->f_halfTileSize*1.5, 
                    rand() % 50, 4, 0.5f, olc::GREY
                );
                particleEmitter->create(
                    worldX + map->f_halfTileSize*1.5, 
                    worldY + map->f_halfTileSize*1.5, 
                    rand() % 50, 4, 0.5f, olc::GREY
                );
            }
        }

        // loop over projectiles, removing dead ones
        if ( !projectiles.empty() )
        {
            // not 100% sure what idx is, possibly an integer index that we can use to break the chain in the list
            // remove_it sorts the projectiles with all that are alive first, followed by all that are not alive
            // then the first not-alive projectile is removed which means all other not-alive projectiles are also
            // deleted
            auto idx = remove_if(projectiles.begin(), projectiles.end(), [&](Projectile *p) {
                return !p->alive;
            });
            if (idx != projectiles.end())
            {
                projectiles.erase(idx);
            }
        }

    }

    void renderMap()
    {
        // The tile map
        for (int y = 0; y < map->mapHeight; y++)
        {
            for (int x = 0; x < map->mapWidth; x++)
            {
                wchar_t tileId = map->getMapTile(x,y);
                if (tileId == L'.')
                {
                    DrawDecal(
                        { x * map->f_tileSize, y * map->f_tileSize },
                        grassDecal
                    );
                }
                else if (tileId == L'#')
                {
                    DrawDecal(
                        { x * map->f_tileSize, y * map->f_tileSize },
                        dirtDecal
                    );
                }
            }
        }

        //TODO change this in favour of a list of obstacles
        for (int y = 0; y < map->mapHeight; y++)
        {
            for (int x = 0; x < map->mapWidth; x++)
            {
                wchar_t obsId = map->getObstacleTile(x,y);
                if (obsId == L'x')
                {
                    DrawDecal({ x * map->f_tileSize, y * map->f_tileSize }, destructableWallDecal);
                }
                else if (obsId == L'o')
                {
                    DrawDecal({ x * map->f_tileSize, y * map->f_tileSize }, solidWallDecal);
                }
            }
        }
    }

    void renderPlayer()
    {

        for (olc::vi2d tile: tilesOfInterest)
        {
            FillRectDecal(
                tile * map->f_tileSize,
                { map->f_tileSize, map->f_tileSize },
                olc::Pixel(255, 0, 0, 64)
            );
        }

        // draw the tank chassis
        DrawRotatedDecal(
            { player->x, player->y },
            tankChassisDecal,
            player->a,
            {map->f_halfTileSize, map->f_halfTileSize },
            {1.0f, 1.0f},
            player->tint
        );

        // draw the turret
        DrawRotatedDecal(
            { player->x, player->y },
            tankTurretDecal,
            player->d,
            {map->f_halfTileSize, map->f_halfTileSize },
            { 1.0f, 1.0f },
            player->tint
        );

    }

    void renderProjectiles()
    {
        for (auto& p : projectiles)
        {
            // Note: PROJECTILE_BULLET_SPREAD doesn't need to be dealt with here as it just creates multiple BULLET objects
            switch (p->type)
            {
            case PROJECTILE_BULLET:
                DrawDecal({ p->x - bulletDrawOffset, p->y - bulletDrawOffset }, bulletDecal);
                break;
            case PROJECTILE_BULLET_AP:
                DrawDecal({ p->x - bulletDrawOffset, p->y - bulletDrawOffset }, bulletDecal, { 1.0f, 1.0f }, olc::RED);
                break;
                //TODO add missile and landmine
            }
        }
    }

    void renderUI()
    {
        // draw player location
        DrawStringDecal(
            { map->f_halfTileSize, map->f_halfTileSize / 2.0f },
            "{ " + std::to_string((int)(player->x / map->f_tileSize)) + " / " + std::to_string((int)(player->y / map->f_tileSize)) + " }"
        );

        float top = ScreenHeight() - map->f_halfTileSize * 1.5;
        float left = map->f_halfTileSize / 2;

        switch(player->currentProjectile)
        {
            case PROJECTILE_BULLET:
                FillRectDecal(
                    { left - 1, top - 1 }, 
                    { 6 * map->f_halfTileSize+2, map->f_halfTileSize+2 },
                    olc::Pixel(0, 0, 0, 96)
                );
                DrawStringDecal(
                    { left, top },
                        "Bullet"
                        );
                break;
            case PROJECTILE_BULLET_AP:
                FillRectDecal(
                    { left - 1, top - 1 }, 
                    { 22 * map->f_halfTileSize + 2, map->f_halfTileSize + 2 },
                    olc::Pixel(0, 0, 0, 96)
                );
                DrawStringDecal(
                    { left, top },
                        "Armour-piercing Bullet"
                );
                break;
            case PROJECTILE_BULLET_SPREAD:
                FillRectDecal(
                    { left - 1, top - 1 },
                    { 13 * map->f_halfTileSize + 2, map->f_halfTileSize + 2 },
                    olc::Pixel(0, 0, 0, 128)
                );
                DrawStringDecal(
                    { left, top },
                    "Spread Bullet"
                );
                break;
            case PROJECTILE_MISSILE:
                FillRectDecal(
                    { left - 1, top - 1 },
                    { 7 * map->f_halfTileSize + 2, map->f_halfTileSize + 2 },
                    olc::Pixel(0, 0, 0, 96)
                );
                DrawStringDecal(
                        { left, top },
                        "Missile"
                );
                break;
            case PROJECTILE_LANDMINE:
                FillRectDecal(
                    { left - 1, top - 1 },
                    { 8 * map->f_halfTileSize + 2, map->f_halfTileSize + 2 },
                    olc::Pixel(0, 0, 0, 96)
                );
                DrawStringDecal(
                        { left, top },
                        "Landmine"
                );
                break;
        }

        // draw progress bar for reloading
        FillRectDecal(
            { 4, map->f_tileSize + 4 },
            { map->f_tileSize - 8, map->mapHeight * map->f_tileSize - map->f_tileSize*2 - 8 },
            olc::Pixel(0, 0, 0, 128)
        );
        if (!player->canShoot())
        {
            // draw progress
            float top = map->f_tileSize + 6;
            float size = (map->mapHeight * map->f_tileSize - map->f_tileSize * 2 - 12) - (player->reloadTime / player->maxReloadTime) * (map->mapHeight * map->f_tileSize - map->f_tileSize * 2 - 12);
            float height = (map->mapHeight * map->f_tileSize - map->f_tileSize * 2 - 12) - size;
            FillRectDecal(
                { 6, top+size },
                { map->f_tileSize - 12, height },
                olc::Pixel(255, 0, 0, 192)
            );
        }

        if( isPaused )
        {
            DrawStringDecal(
                { ScreenWidth()/4.0f, ScreenHeight()/2.0f },
                "Paused. Press (P)"
            );
        }
    }

    void Render( float fElapsedTime )
    {
        renderMap();
        renderPlayer();
        renderProjectiles();
        particleEmitter->render(this);
        renderUI();
    }
};


int main()
{
    TanksGame game;
    if( game.Construct(320, 240, 8, 8, false ) )
    {
        game.Start();
    }
    return 0;
}
