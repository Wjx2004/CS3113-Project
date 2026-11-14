/**
 * Author: Junxiao Wang
 * Assignment: Rise of the AI
 * Date due: 11/16/2026
 * I pledge that I have completed this assignment without
 * collaborating with anyone else, in conformance with the
 * NYU School of Engineering Policies and Procedures on
 * Academic Misconduct.
 */

#include "LevelA.h"

LevelA::LevelA(Vector2 origin, const char *bgHexCode)
    : Scene{ origin, bgHexCode }
{}

void LevelA::initialise()
{
    mGameState.nextSceneID = NO_SCENE;
    for (int column = 0; column < MAP_COLUMNS; column++)
    {
        mLevelData[17 * MAP_COLUMNS + column] = 3;
        mLevelData[18 * MAP_COLUMNS + column] = 23;
    }
    for (int column = 8; column <= 12; column++)
    {
        mLevelData[15 * MAP_COLUMNS + column] = 3;
        mLevelData[16 * MAP_COLUMNS + column] = 23;
    }
    for (int column = 17; column <= 29; column++)
    {
        mLevelData[14 * MAP_COLUMNS + column] = 3;
        mLevelData[15 * MAP_COLUMNS + column] = 23;
        mLevelData[16 * MAP_COLUMNS + column] = 23;
    }
    for (int column = 34; column <= 37; column++)
    {
        mLevelData[15 * MAP_COLUMNS + column] = 3;
        mLevelData[16 * MAP_COLUMNS + column] = 23;
    }
    Vector2 mapOrigin = {
        MAP_COLUMNS * TILE_DIMENSION / 2.0f,
        MAP_ROWS * TILE_DIMENSION / 2.0f
    };


    mGameState.map = new Map(MAP_COLUMNS,MAP_ROWS,mLevelData,"assets/tiles/bamboo_forest_objects.png",TILE_DIMENSION,20,10,mapOrigin);
    mBackground = LoadTexture("assets/backgrounds/bamboo/layer_3.png");

    std::map<Direction, std::vector<int>> runFrames = {
        { LEFT,  { 0, 1, 2, 3, 4, 5, 6, 7 } },
        { RIGHT, { 0, 1, 2, 3, 4, 5, 6, 7 } }
    };
    mGameState.xochitl = new Entity(
        { 100.0f, 440.0f },
        { 160.0f, 160.0f },
        "assets/characters/martial_hero/run.png",
        ATLAS,
        { 1.0f, 8.0f },
        runFrames,
        PLAYER
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

    mEnemies[0] = new Entity(
        { 650.0f, 400.0f },
        { 180.0f, 180.0f },
        "assets/enemies/mushroom/run.png",
        ATLAS,
        { 1.0f, 8.0f },
        runFrames,
        NPC
    );

    mEnemies[0]->setColliderDimensions({ 30.0f, 36.0f });
    mEnemies[0]->setSpeed(70);
    mEnemies[0]->setAcceleration({ 0.0f, 900.0f });
    mEnemies[0]->setAIType(PATROLLER);
    mEnemies[0]->setPatrolRange(590.0f, 910.0f);

    mEnemies[1] = new Entity(
        { 1330.0f, 490.0f },
        { 180.0f, 180.0f },
        "assets/enemies/mushroom/run.png",
        ATLAS,
        { 1.0f, 8.0f },
        runFrames,
        NPC
    );
    mEnemies[1]->setColliderDimensions({ 30.0f, 36.0f });
    mEnemies[1]->setSpeed(85);
    mEnemies[1]->setAcceleration({ 0.0f, 900.0f });
    mEnemies[1]->setAIType(PATROLLER);
    mEnemies[1]->setPatrolRange(1250.0f, 1490.0f);
}

void LevelA::update(float deltaTime)
{
    Entity *player = mGameState.xochitl;

    player->update(deltaTime, nullptr, mGameState.map, nullptr, 0);
    for (int i = 0; i < 2; i++)
        mEnemies[i]->update(deltaTime, player, mGameState.map, nullptr, 0);

    if (player->getPosition().x > 1580.0f)
        mGameState.nextSceneID = LEVEL_B;
}

bool LevelA::playerTouchesEnemy() const
{
    for (int i = 0; i < 2; i++)
        if (mGameState.xochitl->isTouching(mEnemies[i])) return true;
    return false;
}

void LevelA::render()
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
    for (int i = 0; i < 2; i++) mEnemies[i]->render();
    mGameState.xochitl->render();
    DrawText("BAMBOO ENTRANCE", 60, 360, 26, RAYWHITE);
    DrawText("MOUNTAIN GATE >", 1460, 390, 24, RAYWHITE);
}

void LevelA::shutdown()
{
    for (int i = 0; i < 2; i++)
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
