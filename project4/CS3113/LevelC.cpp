/**
 * Author: Junxiao Wang
 * Assignment: Rise of the AI
 * Date due: 11/16/2026
 * I pledge that I have completed this assignment without
 * collaborating with anyone else, in conformance with the
 * NYU School of Engineering Policies and Procedures on
 * Academic Misconduct.
 */

#include "LevelC.h"

LevelC::LevelC(Vector2 origin, const char *bgHexCode)
    : Scene{ origin, bgHexCode }
{}

void LevelC::initialise()
{
    mGameState.nextSceneID = NO_SCENE;

    for (int column = 0; column < MAP_COLUMNS; column++)
    {
        if ((column >= 16 && column <= 17) ||
            (column >= 37 && column <= 39) ||
            (column >= 58 && column <= 59)) continue;

        mLevelData[17 * MAP_COLUMNS + column] = 3;
        mLevelData[18 * MAP_COLUMNS + column] = 23;
    }
    for (int column = 10; column <= 14; column++)
    {
        mLevelData[15 * MAP_COLUMNS + column] = 3;
        mLevelData[16 * MAP_COLUMNS + column] = 23;
    }
    for (int column = 23; column <= 27; column++)
    {
        mLevelData[15 * MAP_COLUMNS + column] = 3;
        mLevelData[16 * MAP_COLUMNS + column] = 23;
    }
    for (int column = 30; column <= 35; column++)
    {
        mLevelData[14 * MAP_COLUMNS + column] = 3;
        mLevelData[15 * MAP_COLUMNS + column] = 23;
        mLevelData[16 * MAP_COLUMNS + column] = 23;
    }
    for (int column = 43; column <= 47; column++)
    {
        mLevelData[15 * MAP_COLUMNS + column] = 3;
        mLevelData[16 * MAP_COLUMNS + column] = 23;
    }
    for (int column = 53; column <= 57; column++)
    {
        mLevelData[15 * MAP_COLUMNS + column] = 3;
        mLevelData[16 * MAP_COLUMNS + column] = 23;
    }
    for (int column = 62; column <= 66; column++)
    {
        mLevelData[15 * MAP_COLUMNS + column] = 3;
        mLevelData[16 * MAP_COLUMNS + column] = 23;
    }

    Vector2 mapOrigin = {
        MAP_COLUMNS * TILE_DIMENSION / 2.0f,
        MAP_ROWS * TILE_DIMENSION / 2.0f
    };
    mGameState.map = new Map(
        MAP_COLUMNS, MAP_ROWS, mLevelData,
        "assets/tiles/bamboo_forest_objects.png",
        TILE_DIMENSION, 20, 10, mapOrigin
    );
    mBackground = LoadTexture("assets/backgrounds/bamboo/layer_1.png");

    std::map<Direction, std::vector<int>> runFrames = {
        { LEFT,  { 0, 1, 2, 3, 4, 5, 6, 7 } },
        { RIGHT, { 0, 1, 2, 3, 4, 5, 6, 7 } }
    };
    mGameState.xochitl = new Entity(
        { 100.0f, 440.0f }, { 160.0f, 160.0f },
        "assets/characters/martial_hero/run.png",
        ATLAS, { 1.0f, 8.0f }, runFrames, PLAYER
    );
    mGameState.xochitl->setColliderDimensions({ 30.0f, 50.0f });
    mGameState.xochitl->setSpeed(200);
    mGameState.xochitl->setJumpingPower(430.0f);
    mGameState.xochitl->setAcceleration({ 0.0f, 900.0f });
    mGameState.xochitl->setPlayerAnimationTextures(
        "assets/characters/martial_hero/idle.png",
        "assets/characters/martial_hero/jump.png",
        "assets/characters/martial_hero/fall.png"
    );

    const float enemyX[3] = { 780.0f, 1600.0f, 2150.0f };
    const float leftEdge[3] = { 650.0f, 1380.0f, 1990.0f };
    const float rightEdge[3] = { 1090.0f, 1800.0f, 2270.0f };

    for (int i = 0; i < 3; i++)
    {
        mEnemies[i] = new Entity(
            { enemyX[i], 370.0f }, { 240.0f, 240.0f },
            "assets/enemies/flying_eye/flight.png",
            ATLAS, { 1.0f, 8.0f }, runFrames, NPC
        );
        mEnemies[i]->setColliderDimensions({ 42.0f, 34.0f });
        mEnemies[i]->setSpeed(80);
        mEnemies[i]->setAIType(FLYER);
        mEnemies[i]->setFlyRange(
            leftEdge[i], rightEdge[i], 270.0f, 450.0f
        );
    }
}

void LevelC::update(float deltaTime)
{
    Entity *player = mGameState.xochitl;
    player->update(deltaTime, nullptr, mGameState.map, nullptr, 0);

    for (int i = 0; i < 3; i++)
        mEnemies[i]->update(deltaTime, player, mGameState.map, nullptr, 0);

    if (player->getPosition().x > 2300.0f)
        mGameState.nextSceneID = WIN_SCENE;
}

bool LevelC::playerTouchesEnemy() const
{
    for (int i = 0; i < 3; i++)
        if (mGameState.xochitl->isTouching(mEnemies[i])) return true;
    return false;
}

void LevelC::render()
{
    ClearBackground(ColorFromHex(mBGColourHexCode));
    for (int x = 0; x < MAP_COLUMNS * TILE_DIMENSION; x += 960)
        DrawTexturePro(
            mBackground,
            { 0.0f, 0.0f, 320.0f, 180.0f },
            { static_cast<float>(x), 0.0f, 960.0f, 540.0f },
            { 0.0f, 0.0f }, 0.0f, WHITE
        );

    mGameState.map->render();
    for (int i = 0; i < 3; i++) mEnemies[i]->render();
    mGameState.xochitl->render();
    DrawText("ANCIENT TEMPLE", 60, 360, 26, RAYWHITE);
    DrawText("TRIAL COMPLETE >", 2220, 390, 24, RAYWHITE);
}

void LevelC::shutdown()
{
    for (int i = 0; i < 3; i++)
    {
        delete mEnemies[i];
        mEnemies[i] = nullptr;
    }
    delete mGameState.xochitl;
    mGameState.xochitl = nullptr;
    delete mGameState.map;
    mGameState.map = nullptr;
    UnloadTexture(mBackground);
    mBackground = {};
}
