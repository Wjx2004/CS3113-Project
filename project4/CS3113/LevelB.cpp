/**
 * Author: Junxiao Wang
 * Assignment: Rise of the AI
 * Date due: 11/16/2026
 * I pledge that I have completed this assignment without
 * collaborating with anyone else, in conformance with the
 * NYU School of Engineering Policies and Procedures on
 * Academic Misconduct.
 */

#include "LevelB.h"

LevelB::LevelB(Vector2 origin, const char *bgHexCode)
    : Scene{ origin, bgHexCode }
{}

void LevelB::initialise()
{
    mGameState.nextSceneID = NO_SCENE;

    for (int column = 0; column < MAP_COLUMNS; column++)
    {
        if ((column >= 20 && column <= 21) ||
            (column >= 44 && column <= 45)) continue;

        mLevelData[17 * MAP_COLUMNS + column] = 3;
        mLevelData[18 * MAP_COLUMNS + column] = 23;
    }
    for (int column = 14; column <= 18; column++)
    {
        mLevelData[15 * MAP_COLUMNS + column] = 3;
        mLevelData[16 * MAP_COLUMNS + column] = 23;
    }
    for (int column = 26; column <= 30; column++)
    {
        mLevelData[15 * MAP_COLUMNS + column] = 3;
        mLevelData[16 * MAP_COLUMNS + column] = 23;
    }
    for (int column = 39; column <= 42; column++)
    {
        mLevelData[14 * MAP_COLUMNS + column] = 3;
        mLevelData[15 * MAP_COLUMNS + column] = 23;
        mLevelData[16 * MAP_COLUMNS + column] = 23;
    }
    for (int column = 52; column <= 56; column++)
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
    mBackground = LoadTexture("assets/backgrounds/bamboo/layer_2.png");

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

    const float enemyX[3] = { 350.0f, 1080.0f, 1870.0f };
    const float leftEdge[3] = { 90.0f, 760.0f, 1500.0f };
    const float rightEdge[3] = { 590.0f, 1310.0f, 2150.0f };

    for (int i = 0; i < 3; i++)
    {
        mEnemies[i] = new Entity(
            { enemyX[i], 490.0f }, { 180.0f, 180.0f },
            "assets/enemies/goblin/run.png",
            ATLAS, { 1.0f, 8.0f }, runFrames, NPC
        );
        mEnemies[i]->setColliderDimensions({ 34.0f, 42.0f });
        mEnemies[i]->setSpeed(95);
        mEnemies[i]->setAcceleration({ 0.0f, 900.0f });
        mEnemies[i]->setAIType(FOLLOWER);
        mEnemies[i]->setPatrolRange(leftEdge[i], rightEdge[i]);
    }
}
void LevelB::update(float deltaTime)
{
    Entity *player = mGameState.xochitl;
    player->update(deltaTime, nullptr, mGameState.map, nullptr, 0);
    for (int i = 0; i < 3; i++)
        mEnemies[i]->update(deltaTime, player, mGameState.map, nullptr, 0);
    if (player->getPosition().x > 2180.0f)
        mGameState.nextSceneID = LEVEL_C;
}

bool LevelB::playerTouchesEnemy() const
{
    for (int i = 0; i < 3; i++)
        if (mGameState.xochitl->isTouching(mEnemies[i])) return true;
    return false;
}

void LevelB::render()
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
    DrawText("MOUNTAIN PATH", 60, 360, 26, RAYWHITE);
    DrawText("TEMPLE >", 2080, 390, 24, RAYWHITE);
}

void LevelB::shutdown()
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
