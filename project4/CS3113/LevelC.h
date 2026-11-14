#ifndef LEVELC_H
#define LEVELC_H

#include "Scene.h"

class LevelC : public Scene
{
private:
    static constexpr int MAP_COLUMNS = 76;
    static constexpr int MAP_ROWS = 19;
    static constexpr float TILE_DIMENSION = 32.0f;
    unsigned int mLevelData[MAP_COLUMNS * MAP_ROWS] = {};
    Entity *mEnemies[3] = {};
    Texture2D mBackground = {};

public:
    LevelC(Vector2 origin, const char *bgHexCode);

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
    bool playerTouchesEnemy() const override;
};

#endif // LEVELC_H
