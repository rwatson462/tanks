#define _USE_MATH_DEFINES
#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine/olcPixelGameEngine.h"
#include <cmath>
#include "projectile.cpp"

constexpr int PROJECTILE_BULLET    = 1;
constexpr int PROJECTILE_BULLET_AP = 2;
constexpr int PROJECTILE_MISSILE   = 3;


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
    float tankRotateSpeed = 2.0f, turretRotateSpeed = 1.0f;
    float tankMoveSpeed = 40.0f;

    std::vector<olc::vi2d> tracks;

    float radToDeg(float rad)
    {
        return double(rad) * 180 / M_PI;
    }

    bool OnUserCreate() override
    {
        mapWidth = ScreenWidth() / tileSize;
        mapHeight = ScreenHeight() / tileSize;

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

        playerX = ScreenWidth() / 2;
        playerY = ScreenHeight() - tileSize*2;
        playerDSin = sin(playerD);
        playerDCos = cos(playerD);

        playerColour = olc::Pixel(0, 255, 255, 255);

        // small map
        map =  L"####################";
        map += L"#..................#";
        map += L"#..................#";
        map += L"#..................#";
        map += L"#..................#";
        map += L"#..................#";
        map += L"#..................#";
        map += L"#..................#";
        map += L"#..................#";
        map += L"#..................#";
        map += L"#..................#";
        map += L"#..................#";
        map += L"#..................#";
        map += L"#..................#";
        map += L"####################";

        // obstacle map
        obsMap =  L"                    ";
        obsMap += L"                    ";
        obsMap += L"   oo xx oo xx oo   ";
        obsMap += L"   o            o   ";
        obsMap += L"   o            o   ";
        obsMap += L"   o            o   ";
        obsMap += L"   o            o   ";
        obsMap += L"   o            o   ";
        obsMap += L"   oo xx oo xx oo   ";
        obsMap += L"                    ";
        obsMap += L"                    ";
        obsMap += L"                    ";
        obsMap += L"  oooooooooooooooo  ";
        obsMap += L"                    ";
        obsMap += L"                    ";

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
        if( GetKey(olc::P ).bPressed )
        {
            isPaused = !isPaused;
        }

        if( isPaused )
        {
            return;
        }

        handlePlayerMovement(fElapsedTime);
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

    void moveTank(float fElapsedTime)
    {
        // player movement
        if (GetKey(olc::W).bHeld)
        {
            // move forwards
            playerX += fElapsedTime * tankMoveSpeed * playerDSin;
            playerY -= fElapsedTime * tankMoveSpeed * playerDCos;
        }
        if (GetKey(olc::S).bHeld)
        {
            // move backwards
            playerX -= fElapsedTime * tankMoveSpeed * playerDSin;
            playerY += fElapsedTime * tankMoveSpeed * playerDCos;
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

    void shoot(float fElapsedTime)
    {
        // left mouse = bullet
        if (GetMouse( 0 ).bPressed )
        {
            // attempt to fire a bullet

            // adjust start position of bullet so it appears to come from the end of the turret
            float startX = playerX + 8.0f * sin(playerA);
            float startY = playerY - 8.0f * cos(playerA);

            Projectile* b = new Projectile(PROJECTILE_BULLET, startX, startY, playerA, 200.0f, 0.2f);
            projectiles.push_back( b );
        }

        // right mouse = special bullet
        if (GetMouse(1).bPressed)
        {
            // fire something else

            // adjust start position of bullet so it appears to come from the end of the turret
            float startX = playerX + 8.0f * sin(playerA);
            float startY = playerY - 8.0f * cos(playerA);

            Projectile* b = new Projectile(PROJECTILE_BULLET_AP, startX, startY, playerA, 150.0f, 2.0f);
            projectiles.push_back(b);
        }

        // space bar = missile
        if (GetKey(olc::SPACE).bPressed)
        {
            float startX = playerX + 8.0f * sin(playerA);
            float startY = playerY - 8.0f * cos(playerA);

            Projectile* b = new Projectile(PROJECTILE_MISSILE, startX, startY, playerA, 50.0f, 10.0f);
            projectiles.push_back(b);
        }
    }

    void updateProjectiles(float fElapsedTime)
    {
        // move each bullet
        for (auto &projectile : projectiles)
        {
            projectile->update(fElapsedTime);

            // check if projectile is off screen
            if (projectile->x < 0 || projectile->x >= ScreenWidth() || projectile->y < 0 || projectile->y > ScreenHeight())
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
        if (GetKey(olc::ESCAPE).bPressed)
        {
            isRunning = false;
            return;
        }

        rotateChassis(fElapsedTime);
        moveTank(fElapsedTime);
        rotateTurret(fElapsedTime);
        shoot(fElapsedTime);

        updateProjectiles( fElapsedTime);
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
            if (p->type == PROJECTILE_BULLET)
            {
                DrawDecal({ p->x - bulletDrawOffset, p->y - bulletDrawOffset }, bulletDecal);
            }
            else if (p->type == PROJECTILE_BULLET_AP)
            {
                DrawDecal({ p->x - bulletDrawOffset, p->y - bulletDrawOffset }, bulletDecal, { 1.0f, 1.0f }, olc::RED);
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

        DrawStringDecal(
            { (float)halfTileSize, (float)tileSize+halfTileSize},
            std::to_string(GetMouseX()) + ":" + std::to_string(GetMouseY())
        );

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
