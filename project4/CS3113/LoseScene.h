#ifndef LOSE_SCENE_H
#define LOSE_SCENE_H

#include "Scene.h"

class LoseScene : public Scene
{
public:
    LoseScene(Vector2 origin, const char *bgHexCode);

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif // LOSE_SCENE_H
