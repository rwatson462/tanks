#define _USE_MATH_DEFINES
#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine/olcPixelGameEngine.h"
#include "map.h"
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

    std::wstring map, obsMap;
    int mapWidth, mapHeight;
    int tileSize = 16, halfTileSize = 8;
    float f_tileSize = (float)tileSize;
    float f_tankBoundingSize = f_tileSize * 0.4f; // distance from centre of tank to edge of hit box
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
    int bulletDrawOffset;
    std::vector<Projectile*> projectiles;

    float playerX = 0.0f, playerY = 0.0f, playerD = 0.0f, playerA = 0.0f;
    float playerDSin = 0.0f, playerDCos = 0.0f;
    olc::Pixel playerColour;
    float tankRotateSpeed = 2.0f;
    float tankMoveSpeed = 40.0f;
    int currentProjectile = PROJECTILE_BULLET;

    std::vector<olc::vi2d> tilesOfInterest;

    std::vector<olc::vi2d> tracks;

    bool OnUserCreate() override
    {
        mapWidth = 20;
        mapHeight = 15;
        tileSize = ScreenHeight() / mapHeight;
        f_tileSize = (float)tileSize;
        halfTileSize = tileSize / 2;

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
        bulletDrawOffset = bulletSprite->width / 2;

        playerX = mapWidth * f_tileSize / 2.0f;
        playerY = mapHeight * f_tileSize - f_tileSize * 2.0f + halfTileSize;
        playerDSin = sin(playerD);
        playerDCos = cos(playerD);

        playerColour = olc::Pixel(255, 128, 255, 255);

        // small map
        loadMap(map, "starting map");

        // obstacle map
        loadMapObstacles(obsMap, "starting map");

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

        handlePlayerMovement(fElapsedTime);
        changeWeapon();
        shoot();
        updateProjectiles(fElapsedTime);
    }

    void changeWeapon()
    {
        if( !GetKey(olc::TAB).bPressed) return;

        switch(currentProjectile)
        {
            case PROJECTILE_BULLET:
                currentProjectile = PROJECTILE_BULLET_AP;
                break;
            case PROJECTILE_BULLET_AP:
                currentProjectile = PROJECTILE_MISSILE;
                break;
            case PROJECTILE_MISSILE:
                currentProjectile = PROJECTILE_LANDMINE;
                break;
            case PROJECTILE_LANDMINE:
                currentProjectile = PROJECTILE_BULLET;
                break;
        }
    }

    void rotateChassis(float fElapsedTime)
    {
        // tank chassis rotation
        if (GetKey(olc::A).bHeld)
        {
            playerD -= fElapsedTime * tankRotateSpeed;
            if (playerD < 0) playerD += 2 * M_PI;
            playerDSin = sin(playerD);
            playerDCos = cos(playerD);
        }
        if (GetKey(olc::D).bHeld)
        {
            playerD += fElapsedTime * tankRotateSpeed;
            if (playerD > 2 * M_PI) playerD -= 2 * M_PI;
            playerDSin = sin(playerD);
            playerDCos = cos(playerD);
        }
    }

    void checkCollision(float& diffX, float& diffY)
    {
        float newX = playerX + diffX;
        float newY = playerY + diffY;

        // check for see if an obstacle exists in the tile we'd be in if we applied the x/yDiff

        int newX_tile, newY_tile;
        int plrX_tile = (int)playerX / f_tileSize;
        int plrY_tile = (int)playerY / f_tileSize;

        // check edges, not corners

        if (newX < playerX)
        {
            // we're moving west
            newX_tile = (int)((newX - f_tankBoundingSize) / f_tileSize);
            newY_tile = (int)(newY / f_tileSize);
            if (obsMap[newY_tile * mapWidth + newX_tile] != L' ')
            {
                diffX = 0;
            }

        }
        else if (newX > playerX)
        {
            // we're moving east
            newX_tile = (int)((newX + f_tankBoundingSize) / f_tileSize);
            newY_tile = (int)(newY / f_tileSize);
            if (obsMap[newY_tile * mapWidth + newX_tile] != L' ')
            {
                diffX = 0;
            }
        }

        if (newY < playerY)
        {
            // we're moving north
            newX_tile = (int)(newX / f_tileSize);
            newY_tile = (int)((newY + f_tankBoundingSize) / f_tileSize);
            printf("%i %i\n", newX_tile, newY_tile);
            if (obsMap[newY_tile * mapWidth + newX_tile] != L' ')
            {
                diffY = 0;
            }

        }
        else if (newY > playerY)
        {
            // we're moving east
            newX_tile = (int)(newX / f_tileSize);
            newY_tile = (int)((newY - f_tankBoundingSize) / f_tileSize);
            if (obsMap[newY_tile * mapWidth + newX_tile] != L' ')
            {
                diffY = 0;
            }
        }

    }

    void moveTank(float fElapsedTime)
    {
        // player movement
        if (GetKey(olc::W).bHeld)
        {
            // move forwards
            float diffX = fElapsedTime * tankMoveSpeed * playerDSin;
            float diffY = fElapsedTime * tankMoveSpeed * playerDCos;
            float newX = playerX + diffX;
            float newY = playerY - diffY;

            // check if the tank is leaving the world
            if (newX < halfTileSize || newX + halfTileSize > mapWidth * f_tileSize)
            {
                diffX = 0;
            }

            if (newY < halfTileSize || newY + halfTileSize > mapHeight * f_tileSize)
            {
                diffY = 0;
            }

            // diffX and diffY are passed by reference so will be updated if needed
            checkCollision(diffX, diffY);

            // we're allowed to move
            playerX += diffX;
            playerY -= diffY;
        }

        if (GetKey(olc::S).bHeld)
        {
            // move backwards
            float diffX = fElapsedTime * tankMoveSpeed * playerDSin;
            float diffY = -fElapsedTime * tankMoveSpeed * playerDCos;
            float newX = playerX + diffX;
            float newY = playerY + diffY;

            // check if the tank is leaving the world
            if (newX < halfTileSize || newX + halfTileSize > mapWidth * f_tileSize)
            {
                diffX = 0;
            }

            if (newY < halfTileSize || newY + halfTileSize > mapHeight * f_tileSize)
            {
                diffY = 0;
            }

            checkCollision(diffX, diffY);

            // make the move
            playerX -= diffX;
            playerY -= diffY;
        }
    }

    void rotateTurret(float fElapsedTime)
    {
        // turret always points at mouse

        // calculate x/y distances from player to mouse
        float xD = GetMouseX() - float(playerX);
        float yD = -(GetMouseY() - float(playerY));

        // handle possible divide by zero errors
        if (yD == 0)
        {
            if (xD > 0)
            {
                playerA = M_PI * 0.5;
            }
            else if (xD < 0)
            {
                playerA = M_PI * 1.5;
            }
            else
            {
                playerA = 0;
            }
        }
        else
        {
            playerA = atan(xD / yD);
            if (yD < 0) playerA += M_PI;
        }
    }

    void shoot()
    {
        // left mouse = bullet
        if (GetMouse( 0 ).bPressed || GetKey(olc::SPACE).bPressed)
        {
            // attempt to fire whatever the current projectile is

            // adjust start position of bullet so it appears to come from the end of the turret
            float startX = playerX + 8.0f * sin(playerA);
            float startY = playerY - 8.0f * cos(playerA);

            // TODO create struct of all projectiles with their stats so we don't need to refer to all of them in here
            Projectile* b;

            switch( currentProjectile )
            {
                case PROJECTILE_BULLET:
                    b = new Projectile(PROJECTILE_BULLET, startX, startY, playerA, 200.0f, 0.2f);
                    break;
                case PROJECTILE_BULLET_AP:
                    b = new Projectile(PROJECTILE_BULLET_AP, startX, startY, playerA, 150.0f, 2.0f);
                    break;
                case PROJECTILE_MISSILE:
                    b = new Projectile(PROJECTILE_MISSILE, startX, startY, playerA, 50.0f, 10.0f);
                    break;
                case PROJECTILE_LANDMINE:
                    b = new Projectile(PROJECTILE_LANDMINE, playerX, playerY, playerA, 0.0f, 10.0f);
                    break;
            }

            projectiles.push_back( b );
        }
    }

    void updateProjectiles(float fElapsedTime)
    {
        // move each bullet
        for (auto &projectile : projectiles)
        {
            projectile->update(fElapsedTime);

            // check if projectile is off screen
            if (projectile->x < 0 || projectile->x >= mapWidth * f_tileSize || projectile->y < 0 || projectile->y > mapHeight * f_tileSize)
            {
                projectile->isAlive = false;
                continue;
            }

            // check collisions with obstacles and other tanks

            // convert projectile's x/y to map coords
            int projX = floor(projectile->x / tileSize);
            int projY = floor(projectile->y / tileSize);

            wchar_t obs = obsMap[projY * mapWidth + projX];
            if (obs == L'o')
            {
                // we're colliding with a solid wall, just delete ourselves
                projectile->isAlive = false;
            }
            else if (obs == L'x')
            {
                // we're colliding with a destructable wall, destruct it
                projectile->isAlive = false;
                obsMap[projY * mapWidth + projX] = L' ';
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
                return !p->isAlive;
            });
            if (idx != projectiles.end())
            {
                projectiles.erase(idx);
            }
        }

    }

    void handlePlayerMovement(float fElapsedTime)
    {
        // handle A/D
        rotateChassis(fElapsedTime);

        // handle W/S
        moveTank(fElapsedTime);

        rotateTurret(fElapsedTime);
    }

    void renderMap()
    {
        // The tile map
        for (int y = 0; y < mapHeight; y++)
        {
            for (int x = 0; x < mapWidth; x++)
            {
                wchar_t tileId = map[y * mapWidth + x];
                if (tileId == L'.')
                {
                    DrawDecal(
                        { (float)(x * tileSize), float(y * tileSize) },
                        grassDecal
                    );
                }
                else if (tileId == L'#')
                {
                    DrawDecal(
                        { float(x * tileSize), float(y * tileSize) },
                        dirtDecal
                    );
                }
            }
        }

        //TODO change this in favour of a list of obstacles
        for (int y = 0; y < mapHeight; y++)
        {
            for (int x = 0; x < mapWidth; x++)
            {
                wchar_t obsId = obsMap[y * mapWidth + x];
                if (obsId == L'x')
                {
                    DrawDecal({ float(x * tileSize), float(y * tileSize) }, destructableWallDecal);
                }
                else if (obsId == L'o')
                {
                    DrawDecal({ float(x * tileSize), float(y * tileSize) }, solidWallDecal);
                }
            }
        }

    }

    void renderPlayer()
    {

        for (olc::vi2d tile: tilesOfInterest)
        {
            FillRectDecal(
                tile * f_tileSize,
                { f_tileSize, f_tileSize },
                olc::Pixel(255, 0, 0, 64)
            );
        }

        // draw the tank chassis
        DrawRotatedDecal(
            { playerX, playerY },
            tankChassisDecal,
            playerD,
            { float(halfTileSize), float(halfTileSize) },
            {1.0f, 1.0f},
            playerColour
        );

        // draw the turret
        DrawRotatedDecal(
            { playerX, playerY },
            tankTurretDecal,
            playerA,
            { float(halfTileSize), float(halfTileSize) },
            { 1.0f, 1.0f },
            playerColour
        );

    }

    void renderBullets()
    {
        for (auto& p : projectiles)
        {
            switch( p->type)
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
            { (float)halfTileSize, (float)halfTileSize },
            "{ " + std::to_string((int)playerX/tileSize) + " / " + std::to_string((int)playerY/tileSize) + " }",
            olc::WHITE
        );

        switch(currentProjectile)
        {
            case PROJECTILE_BULLET:
                DrawStringDecal(
                        {(float)halfTileSize, ScreenHeight() - 12.0f},
                        "Bullet"
                        );
                break;
            case PROJECTILE_BULLET_AP:
                DrawStringDecal(
                        {(float)halfTileSize, ScreenHeight() - 12.0f},
                        "Armour-piercing Bullet"
                );
                break;
            case PROJECTILE_MISSILE:
                DrawStringDecal(
                        {(float)halfTileSize, ScreenHeight() - 12.0f},
                        "Missile"
                );
                break;
            case PROJECTILE_LANDMINE:
                DrawStringDecal(
                        {(float)halfTileSize, ScreenHeight() - 12.0f},
                        "Landmine"
                );
                break;
        }

        if( isPaused )
        {
            DrawStringDecal(
                { (float)ScreenWidth()/4, (float)ScreenHeight()/2 },
                "Paused. Press (P)"
            );
        }
    }

    void Render( float fElapsedTime )
    {
        renderMap();
        renderPlayer();
        renderBullets();
        renderUI();
    }
};



int main()
{
    TanksGame game;
    if( game.Construct(320, 240, 2, 2, true ) )
    {
        game.Start();
    }
    return 0;
}
