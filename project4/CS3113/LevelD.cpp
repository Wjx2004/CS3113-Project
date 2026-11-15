/**
 * Author: Junxiao Wang
 * Assignment: Rise of the AI
 * Date due: 11/16/2026
 * I pledge that I have completed this assignment without
 * collaborating with anyone else, in conformance with the
 * NYU School of Engineering Policies and Procedures on
 * Academic Misconduct.
 */

#include "LevelD.h"

LevelD::LevelD(Vector2 origin, const char *bgHexCode) : Scene{ origin, bgHexCode } {}

void LevelD::initialise()
{
    mGameState.nextSceneID = NO_SCENE;
    mBossHits = 0;
    mHitTime = 0.0f;

    for (int column = 0; column < MAP_COLUMNS; column++)
    {
        mLevelData[17 * MAP_COLUMNS + column] = 3;
        mLevelData[18 * MAP_COLUMNS + column] = 23;
    }
    for (int column = 12; column <= 16; column++)
    {
        mLevelData[15 * MAP_COLUMNS + column] = 3;
        mLevelData[16 * MAP_COLUMNS + column] = 23;
    }
    for (int column = 24; column <= 29; column++)
    {
        mLevelData[15 * MAP_COLUMNS + column] = 3;
        mLevelData[16 * MAP_COLUMNS + column] = 23;
    }
    for (int column = 34; column <= 62; column++)
    {
        mLevelData[14 * MAP_COLUMNS + column] = 3;
        mLevelData[15 * MAP_COLUMNS + column] = 23;
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
    mBossSound = LoadSound("assets/sfx/sword_slash.ogg");

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
    mGameState.xochitl->setJumpingPower(550.0f);
    mGameState.xochitl->setAcceleration({ 0.0f, 900.0f });
    mGameState.xochitl->setPlayerAnimationTextures(
        "assets/characters/martial_hero/idle.png",
        "assets/characters/martial_hero/jump.png",
        "assets/characters/martial_hero/fall.png"
    );

    mBoss = new Entity(
        { 1600.0f, 355.0f }, { 360.0f, 360.0f },
        "assets/enemies/flying_eye/flight.png",
        ATLAS, { 1.0f, 8.0f }, runFrames, NPC
    );
    mBoss->setColliderDimensions({ 100.0f, 50.0f });
    mBoss->setSpeed(55);
    mBoss->setAIType(FLYER);
    mBoss->setFlyRange(1250.0f, 1850.0f, 350.0f, 360.0f);
}

void LevelD::update(float deltaTime)
{
    Entity *player = mGameState.xochitl;
    player->update(deltaTime, nullptr, mGameState.map, nullptr, 0);
    mBoss->update(deltaTime, player, mGameState.map, nullptr, 0);
    if (mHitTime > 0.0f)
    {
        mHitTime -= deltaTime;
        return;
    }
    if (!player->isTouching(mBoss)) return;

    Vector2 playerPosition = player->getPosition();
    Vector2 bossPosition = mBoss->getPosition();
    float playerBottom = playerPosition.y +
        player->getColliderDimensions().y / 2.0f;
    float bossTop = bossPosition.y -
        mBoss->getColliderDimensions().y / 2.0f;

    if (player->getVelocity().y > 0.0f &&
        playerBottom < bossTop + 14.0f)
    {
        mBossHits++;
        mHitTime = 0.45f;
        player->setPosition({ playerPosition.x,bossTop - player->getColliderDimensions().y / 2.0f - 2.0f });
        player->setVelocity({ player->getVelocity().x, -360.0f });
        PlaySound(mBossSound);

        if (mBossHits == 3)
        {
            mBoss->deactivate();
            mGameState.nextSceneID = WIN_SCENE;
        }
        else
            mBoss->setSpeed(55 + 15 * mBossHits);
    }
}

bool LevelD::playerTouchesEnemy() const
{
    return mHitTime <= 0.0f &&
           mGameState.xochitl->isTouching(mBoss);
}

void LevelD::render()
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
    mBoss->render();
    mGameState.xochitl->render();
    Vector2 bossPosition = mBoss->getPosition();
    DrawRectangle(
        static_cast<int>(bossPosition.x) - 90,
        static_cast<int>(bossPosition.y) - 100,
        180, 12, DARKGRAY
    );

    DrawRectangle(
        static_cast<int>(bossPosition.x) - 90,
        static_cast<int>(bossPosition.y) - 100,
        60 * (3 - mBossHits), 12, RED
    );
    DrawText("ANCIENT SANCTUARY", 100, 350, 28, RAYWHITE);
    DrawText("JUMP ABOVE THE GUARDIAN AND LAND ON ITS HEAD 3 TIMES",
             1020, 280, 22, RAYWHITE);
}
void LevelD::shutdown()
{
    delete mBoss;
    mBoss = nullptr;
    delete mGameState.xochitl;
    mGameState.xochitl = nullptr;
    delete mGameState.map;
    mGameState.map = nullptr;
    UnloadSound(mBossSound);
    mBossSound = {};
    UnloadTexture(mBackground);
    mBackground = {};
}
