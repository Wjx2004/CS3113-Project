/**
 * Author: Junxiao Wang
 * Assignment: Lunar Lander!
 * Date due: 10/26/2026
 * I pledge that I have completed this assignment without
 * collaborating with anyone else, in conformance with the
 * NYU School of Engineering Policies and Procedures on
 * Academic Misconduct.
 */


#include "CS3113/Entity.h"
#include <string>


constexpr int SCREEN_WIDTH  = 1000,
              SCREEN_HEIGHT = 650,
              FPS           = 120;

constexpr float SIDE_THRUST = 240.0f;
constexpr float UP_THRUST = 200.0f;
constexpr float RELEASE_DECELERATION = 145.0f;
constexpr float MAX_FUEL = 100.0f;
constexpr float FUEL_PER_SECOND = 2.0f;
constexpr float PLATFORM_SPEED = 32.0f;

constexpr char    BG_COLOUR[] = "#423d3c";
constexpr Vector2 ORIGIN      = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };

constexpr int NUMBER_OF_FLOOR_TILES = 28;
constexpr int NUMBER_OF_PLATFORM_TILES = 7;
constexpr int LEFT_WALL_COLUMNS = 4;
constexpr int RIGHT_WALL_COLUMNS = 3;
constexpr int WALL_ROWS = 7;
constexpr int NUMBER_OF_WALL_TILES = (LEFT_WALL_COLUMNS + RIGHT_WALL_COLUMNS) * WALL_ROWS;
constexpr int NUMBER_OF_TILES = NUMBER_OF_FLOOR_TILES + NUMBER_OF_PLATFORM_TILES + NUMBER_OF_WALL_TILES;

constexpr float TILE_DIMENSION          = 36.0f,
                ACCELERATION_OF_GRAVITY = 55.0f,
                FIXED_TIMESTEP          = 1.0f / 60.0f;
// Global Variables
AppStatus gAppStatus = RUNNING;

float gPreviousTicks   = 0.0f,
      gTimeAccumulator = 0.0f;

Entity *gProtag = nullptr;
Entity *gTiles  = nullptr;
Entity *leftFlame = nullptr;
Entity *rightFlame = nullptr;
Entity *tailFlame = nullptr;
Texture2D landingSignTexture{};
Texture2D hazardBeaconTexture{};
Texture2D backgroundTexture{};
bool aKeyDown = false;
bool dKeyDown = false;
bool spaceKeyDown = false;
float fuel = MAX_FUEL;
float platformDirection = -1.0f;
bool missionEnded = false;
bool missionWon = false;

// Function Declarations
void initialise();
void processInput();
void update();
void render();
void shutdown();

void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Cosmic Parking");
    backgroundTexture = LoadTexture("assets/starfield_blue.png");
    landingSignTexture = LoadTexture("assets/landing_sign_green.png");
    hazardBeaconTexture = LoadTexture("assets/hazard_beacon_red.png");
    gProtag = new Entity(
        { ORIGIN.x, ORIGIN.y - 200.0f },
        { 132.0f, 44.0f },
        "assets/car_yellow.png"
    );

    gProtag->setColliderDimensions({ 120.0f, 34.0f });
    gProtag->setAcceleration({0.0f,ACCELERATION_OF_GRAVITY});

    const std::map<Direction, std::vector<int>> flameFrames = {
        { RIGHT, { 0, 1, 2 } }
    };

    Vector2 carPosition = gProtag->getPosition();
    Vector2 flameSize = { 20.0f, 44.0f };
    Vector2 sheetDimensions = { 1.0f, 3.0f };

    Vector2 leftFlamePosition = {carPosition.x - 33.0f,carPosition.y + 37.0f};

    Vector2 rightFlamePosition = {carPosition.x + 33.0f,carPosition.y + 37.0f};

    leftFlame = new Entity(
        leftFlamePosition,
        flameSize,
        "assets/thrust_sheet.png",
        ATLAS,
        sheetDimensions,
        flameFrames
    );

    rightFlame = new Entity(
        rightFlamePosition,
        flameSize,
        "assets/thrust_sheet.png",
        ATLAS,
        sheetDimensions,
        flameFrames
    );

    tailFlame = new Entity(
        { carPosition.x - 78.0f, carPosition.y + 2.0f },
        flameSize,
        "assets/thrust_sheet.png",
        ATLAS,
        sheetDimensions,
        flameFrames
    );
    tailFlame->setAngle(90.0f);

    gTiles = new Entity[NUMBER_OF_TILES];

    float leftMostX = TILE_DIMENSION / 2.0f;
    float floorY =SCREEN_HEIGHT - 32.0f - TILE_DIMENSION / 2.0f;

    for (int i = 0; i < NUMBER_OF_FLOOR_TILES; i++)
    {
        gTiles[i].setTexture("assets/ground_metal.png");
        gTiles[i].setScale({TILE_DIMENSION,TILE_DIMENSION});
        gTiles[i].setColliderDimensions({TILE_DIMENSION,TILE_DIMENSION});
        gTiles[i].setPosition({leftMostX + i * TILE_DIMENSION,floorY});
    }
    float firstPlatformTileX = 580.0f + TILE_DIMENSION / 2.0f;
    float platformY = 490.0f;

    for (int i = 0; i < NUMBER_OF_PLATFORM_TILES; i++)
    {
        int platformTileIndex = NUMBER_OF_FLOOR_TILES + i;

        if (i == 0)
            gTiles[platformTileIndex].setTexture("assets/platform_left.png");
        else if (i == NUMBER_OF_PLATFORM_TILES - 1)
            gTiles[platformTileIndex].setTexture("assets/platform_right.png");
        else
            gTiles[platformTileIndex].setTexture("assets/platform_middle.png");

        gTiles[platformTileIndex].setScale({TILE_DIMENSION,TILE_DIMENSION});
        gTiles[platformTileIndex].setColliderDimensions({TILE_DIMENSION,TILE_DIMENSION});
        gTiles[platformTileIndex].setPosition({firstPlatformTileX + i * TILE_DIMENSION,platformY});
    }
    int wallTileIndex =NUMBER_OF_FLOOR_TILES + NUMBER_OF_PLATFORM_TILES;

    for (int wallNumber = 0; wallNumber < 2; wallNumber++)
    {
        int wallColumns = LEFT_WALL_COLUMNS;
        float firstWallTileX = TILE_DIMENSION / 2.0f;
        float firstWallTileY = 366.0f;

        if (wallNumber == 1)
        {
            wallColumns = RIGHT_WALL_COLUMNS;
            firstWallTileX = SCREEN_WIDTH - RIGHT_WALL_COLUMNS * TILE_DIMENSION + TILE_DIMENSION / 2.0f;
            firstWallTileY = 348.0f;
        }

        for (int row = 0; row < WALL_ROWS; row++)
        {
            for (int column = 0; column < wallColumns; column++)
            {
                if (row == 0)
                    gTiles[wallTileIndex].setTexture("assets/hazard_panel.png");
                else
                    gTiles[wallTileIndex].setTexture("assets/ground_metal.png");

                gTiles[wallTileIndex].setScale({
                    TILE_DIMENSION,
                    TILE_DIMENSION
                });
                gTiles[wallTileIndex].setColliderDimensions({
                    TILE_DIMENSION,
                    TILE_DIMENSION
                });
                gTiles[wallTileIndex].setPosition({
                    firstWallTileX + column * TILE_DIMENSION,
                    firstWallTileY + row * TILE_DIMENSION
                });

                wallTileIndex++;
            }
        }
    }
    SetTargetFPS(FPS);
    gPreviousTicks = static_cast<float>(GetTime());
}

void processInput()
{
    if (IsKeyPressed(KEY_Q) || WindowShouldClose())
        gAppStatus = TERMINATED;
    aKeyDown = IsKeyDown(KEY_A);
    dKeyDown = IsKeyDown(KEY_D);
    spaceKeyDown = IsKeyDown(KEY_SPACE);
}
void update()
{
    if (missionEnded) return;

    float ticks = static_cast<float>(GetTime());
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks  = ticks;

    deltaTime += gTimeAccumulator;

    if (deltaTime < FIXED_TIMESTEP)
    {
        gTimeAccumulator = deltaTime;
        return;
    }

    while (deltaTime >= FIXED_TIMESTEP)
    {   
        float firstPlatformTileX =
            gTiles[NUMBER_OF_FLOOR_TILES].getPosition().x;

        if (firstPlatformTileX <= 500.0f || firstPlatformTileX >= 650.0f)
            platformDirection = -platformDirection;

        for (int i = 0; i < NUMBER_OF_PLATFORM_TILES; i++)
        {
            int tileIndex = NUMBER_OF_FLOOR_TILES + i;
            Vector2 position = gTiles[tileIndex].getPosition();

            position.x += platformDirection * PLATFORM_SPEED * FIXED_TIMESTEP;
            gTiles[tileIndex].setPosition(position);
        }
        Vector2 carAcceleration = {0.0f, ACCELERATION_OF_GRAVITY};
        int keysUsingFuel = 0;
        if (fuel > 0.0f)
        {
            if (aKeyDown)
            {
                carAcceleration.x -= SIDE_THRUST;
                keysUsingFuel++;
            }

            if (dKeyDown)
            {
                carAcceleration.x += SIDE_THRUST;
                keysUsingFuel++;
            }

            if (spaceKeyDown)
            {
                carAcceleration.y -= UP_THRUST;
                keysUsingFuel++;
            }
        }
        fuel -= keysUsingFuel * FUEL_PER_SECOND * FIXED_TIMESTEP;
        if (fuel < 0.0f)
            fuel = 0.0f;
        if (carAcceleration.x == 0.0f)
        {
            float horizontalSpeed = gProtag->getVelocity().x;

            if (horizontalSpeed > 0.0f)
            {
                carAcceleration.x = -RELEASE_DECELERATION;

                if (horizontalSpeed + carAcceleration.x * FIXED_TIMESTEP < 0.0f)
                {carAcceleration.x =-horizontalSpeed / FIXED_TIMESTEP;}
            }
            else if (horizontalSpeed < 0.0f)
            {
                carAcceleration.x = RELEASE_DECELERATION;

                if (horizontalSpeed + carAcceleration.x * FIXED_TIMESTEP > 0.0f)
                {carAcceleration.x = -horizontalSpeed / FIXED_TIMESTEP;}
            }
        }
        
        gProtag->setAcceleration(carAcceleration);
        gProtag->update(FIXED_TIMESTEP,gTiles,NUMBER_OF_TILES);
        Vector2 currentCarPosition = gProtag->getPosition();

        leftFlame->setPosition({currentCarPosition.x - 33.0f,currentCarPosition.y + 37.0f});

        rightFlame->setPosition({currentCarPosition.x + 33.0f,currentCarPosition.y + 37.0f});
        tailFlame->setPosition({currentCarPosition.x - 78.0f,currentCarPosition.y + 2.0f});

        leftFlame->update(FIXED_TIMESTEP, nullptr, 0);
        rightFlame->update(FIXED_TIMESTEP, nullptr, 0);
        tailFlame->update(FIXED_TIMESTEP, nullptr, 0);
        if (gProtag->isCollidingLeft() ||gProtag->isCollidingRight() ||gProtag->isCollidingTop())

        {missionWon = false; missionEnded = true;return;}
        
        if (gProtag->isCollidingBottom())
        {
            Vector2 carPosition = gProtag->getPosition();
            float carHalfWidth = gProtag->getColliderDimensions().x / 2.0f;

            float platformLeft = gTiles[NUMBER_OF_FLOOR_TILES].getPosition().x - TILE_DIMENSION / 2.0f;
            float platformRight = gTiles[NUMBER_OF_FLOOR_TILES + NUMBER_OF_PLATFORM_TILES - 1].getPosition().x + TILE_DIMENSION / 2.0f;

            missionWon =
                carPosition.y < gTiles[NUMBER_OF_FLOOR_TILES].getPosition().y
                && carPosition.x - carHalfWidth >= platformLeft
                && carPosition.x + carHalfWidth <= platformRight;

            missionEnded = true;
            return;
        }

        for (int i = 0; i < NUMBER_OF_TILES; i++)
            gTiles[i].update(FIXED_TIMESTEP,nullptr,0);
        deltaTime -= FIXED_TIMESTEP;
    }

    gTimeAccumulator = deltaTime;
}
void render()
{
    BeginDrawing();
    ClearBackground(ColorFromHex(BG_COLOUR));
    Rectangle backgroundSource = {0.0f, 0.0f,static_cast<float>(backgroundTexture.width),static_cast<float>(backgroundTexture.height)};
    for (int y = 0; y < SCREEN_HEIGHT; y += 256)
    {
        for (int x = 0; x < SCREEN_WIDTH; x += 256)
        {
            Rectangle backgroundDestination = {
                static_cast<float>(x),
                static_cast<float>(y),
                256.0f,
                256.0f
            };
            DrawTexturePro(
                backgroundTexture,
                backgroundSource,
                backgroundDestination,
                { 0.0f, 0.0f },
                0.0f,
                WHITE
            );
        }
    }
    if (!missionEnded && fuel > 0.0f)
    {
        if (dKeyDown)
            tailFlame->render();

        if (spaceKeyDown)
        {
            leftFlame->render();
            rightFlame->render();
        }
    }
    gProtag->render();

    for (int i = 0; i < NUMBER_OF_TILES; i++) 
        gTiles[i].render();
    Vector2 firstPlatformTilePosition =
        gTiles[NUMBER_OF_FLOOR_TILES].getPosition();

    Rectangle signSource = {
        0.0f, 0.0f,
        static_cast<float>(landingSignTexture.width),
        static_cast<float>(landingSignTexture.height)
    };
    Rectangle signDestination = {
        firstPlatformTilePosition.x - TILE_DIMENSION / 2.0f + 12.0f,
        firstPlatformTilePosition.y - TILE_DIMENSION / 2.0f - 54.0f,
        54.0f, 54.0f
    };
    DrawTexturePro(
        landingSignTexture, signSource, signDestination,
        { 0.0f, 0.0f }, 0.0f, WHITE
    );

    Rectangle beaconSource = {
        0.0f, 0.0f,
        static_cast<float>(hazardBeaconTexture.width),
        static_cast<float>(hazardBeaconTexture.height)
    };
    Rectangle leftBeaconDestination = { 100.0f, 316.0f, 32.0f, 32.0f };
    Rectangle rightBeaconDestination = { 896.0f, 298.0f, 32.0f, 32.0f };

    DrawTexturePro(
        hazardBeaconTexture, beaconSource, leftBeaconDestination,
        { 0.0f, 0.0f }, 0.0f, WHITE
    );
    DrawTexturePro(
        hazardBeaconTexture, beaconSource, rightBeaconDestination,
        { 0.0f, 0.0f }, 0.0f, WHITE
    );
    DrawRectangle(0,0,SCREEN_WIDTH,80,ColorFromHex("#18202E"));
    DrawText("COSMIC PARKING", 22, 18, 22, RAYWHITE);
    DrawText("LAND ON THE GREEN P", 22, 51, 12, RAYWHITE);

    int fuelNumber = static_cast<int>(fuel);
    if (fuel > 0.0f && fuelNumber == 0)
        fuelNumber = 1;
    std::string fuelText = "FUEL " + std::to_string(fuelNumber) + " / 100";
    DrawText(fuelText.c_str(), 730, 18, 20, RAYWHITE);
    DrawRectangle(730, 50, 220, 10, DARKGRAY);
    
    int fuelBarWidth =static_cast<int>(220.0f * fuel / MAX_FUEL);
    DrawRectangle(730, 50, fuelBarWidth, 10,ColorFromHex("#76DDB5"));
    DrawRectangle(0, 618, SCREEN_WIDTH, 32, ColorFromHex("#18202E"));
    DrawText("A / D: SIDE THRUST   SPACE: LIFT   Q: QUIT",20, 628, 14, RAYWHITE);
    if (missionEnded)
    {
        DrawRectangle(300, 265, 400, 100, ColorFromHex("#18202E"));
        if (missionWon)
            DrawText("Mission Accomplished", 320, 300, 22, RAYWHITE);
        else
            DrawText("Mission Failed", 380, 300, 22, RAYWHITE);
    }

    EndDrawing();
}
void shutdown()
{
    // Unload the textures while the window (GL context) still exists
    delete gProtag;
    delete[] gTiles;
    UnloadTexture(backgroundTexture);
    UnloadTexture(landingSignTexture);
    UnloadTexture(hazardBeaconTexture);
    delete leftFlame;
    delete rightFlame;
    delete tailFlame;
    CloseWindow();
}
int main(void)
{
    initialise();

    while (gAppStatus == RUNNING)
    {
        processInput();
        update();
        render();
    }

    shutdown();
    return 0;
}