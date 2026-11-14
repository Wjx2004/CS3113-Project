#ifndef LEVELB_H
#define LEVELB_H

#include "Scene.h"

class LevelB : public Scene
{
private:
    static constexpr int MAP_COLUMNS = 72;
    static constexpr int MAP_ROWS = 19;
    static constexpr float TILE_DIMENSION = 32.0f;
    unsigned int mLevelData[MAP_COLUMNS * MAP_ROWS] = {};
    Entity *mEnemies[3] = {};
    Texture2D mBackground = {};

public:
    LevelB(Vector2 origin, const char *bgHexCode);

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
    bool playerTouchesEnemy() const override;
};

#endif // LEVELB_H
