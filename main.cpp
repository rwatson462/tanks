#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine/olcPixelGameEngine.h"


class TanksGame : public olc::PixelGameEngine
{
public:
    TanksGame()
    {
        sAppName = "Tanks!";
    }

private:

    std::wstring map;
    int mapWidth, mapHeight;
    int tileSize, halfTileSize;
    olc::Sprite *grassSprite;
    olc::Sprite *dirtSprite;
    olc::Decal *grassDecal;
    olc::Decal *dirtDecal;

    olc::Sprite *tankChassisSprite;
    olc::Sprite *tankTurretSprite;
    olc::Decal *tankChassisDecal;
    olc::Decal *tankTurretDecal;

    float playerX, playerY, playerD, playerA = 0.0f;

    bool OnUserCreate() override
    {
        tileSize = 16;
        halfTileSize = tileSize / 2;
        mapWidth = ScreenWidth() / tileSize;
        mapHeight = ScreenHeight() / tileSize;

        grassSprite = new olc::Sprite("./assets/grasstop.png");
        dirtSprite = new olc::Sprite("./assets/dirt.png");
        grassDecal = new olc::Decal( grassSprite );
        dirtDecal = new olc::Decal( dirtSprite );

        tankChassisSprite = new olc::Sprite( "./assets/tank-chassis.png" );
        tankTurretSprite = new olc::Sprite( "./assets/tank-turret.png" );
        tankChassisDecal = new olc::Decal( tankChassisSprite );
        tankTurretDecal = new olc::Decal( tankTurretSprite );

        playerX = 10;
        playerY = 6;

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

        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        Update( fElapsedTime );
        Render( fElapsedTime );
        return true;
    }


    void Update( float fElapsedTime )
    {
        // normalise angle based on fElapsedTime
        float angle = 1 * fElapsedTime;

        // player rotation
        if( GetKey( olc::A ).bHeld )
        {
            playerD -= angle;
        }
        if( GetKey( olc::D ).bHeld )
        {
            playerD += angle;
        }

        // player movement
        // todo
        // use playerD (direction) to move

        // turret rotation
        if( GetKey( olc::Q ).bHeld )
        {
            playerA -= angle;
        }
        if( GetKey( olc::E ).bHeld )
        {
            playerA += angle;
        }

        // FIRE ZE MISSILES
    }

    void Render( float fElapsedTime )
    {
        // reset screen
        Clear(olc::DARK_GREEN);

        // draw map
        for( int y = 0; y < mapHeight; y ++ )
        {
            for( int x = 0; x < mapWidth; x++ )
            {
                wchar_t tileId = map[ y * mapWidth + x ];
                if( tileId == L'.' )
                {
                    DrawDecal( { float(x*tileSize), float(y*tileSize) }, grassDecal );
                }
                else if( tileId == L'#' )
                {
                    DrawDecal( { float(x*tileSize), float(y*tileSize) }, dirtDecal );
                }
            }
        }

        // draw player
        float playerXt = playerX * tileSize;
        float playerYt = playerY * tileSize;
        DrawRotatedDecal(
            { playerXt, playerYt },
            tankChassisDecal,
            playerD,
            { float(halfTileSize), float(halfTileSize) }
        );
        DrawRotatedDecal(
            { playerXt, playerYt },
            tankTurretDecal,
            playerA,
            { float(halfTileSize), float(halfTileSize) }
        );
    }
};



int main()
{
    TanksGame game;
    if( game.Construct(320,240, 4,4 ) )
    {
        game.Start();
    }
    return 0;
}
