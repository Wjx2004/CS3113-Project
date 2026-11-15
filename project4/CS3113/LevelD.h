#ifndef LEVELD_H
#define LEVELD_H

#include "Scene.h"

class LevelD : public Scene
{
private:
    static constexpr int MAP_COLUMNS = 76;
    static constexpr int MAP_ROWS = 19;
    static constexpr float TILE_DIMENSION = 32.0f;
    unsigned int mLevelData[MAP_COLUMNS * MAP_ROWS] = {};
    Entity *mBoss = nullptr;
    Texture2D mBackground = {};
    Sound mBossSound = {};
    int mBossHits = 0;
    float mHitTime = 0.0f;

public:
    LevelD(Vector2 origin, const char *bgHexCode);

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
    bool playerTouchesEnemy() const override;

    int getBossHits() const { return mBossHits; }
};

#endif
