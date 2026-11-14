#ifndef WIN_SCENE_H
#define WIN_SCENE_H

#include "Scene.h"

class WinScene : public Scene
{
public:
    WinScene(Vector2 origin, const char *bgHexCode);

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif // WIN_SCENE_H
