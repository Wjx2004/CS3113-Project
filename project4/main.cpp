/**
 * Author: Junxiao Wang
 * Assignment: Rise of the AI
 * Date due: 11/16/2026
 * I pledge that I have completed this assignment without
 * collaborating with anyone else, in conformance with the
 * NYU School of Engineering Policies and Procedures on
 * Academic Misconduct.
 */
#include "CS3113/LevelA.h"
#include "CS3113/LevelB.h"
#include "CS3113/LevelC.h"
#include "CS3113/LoseScene.h"
#include "CS3113/Menu.h"
#include "CS3113/WinScene.h"
#include <string>

constexpr int SCREEN_WIDTH = 1000,
              SCREEN_HEIGHT = 600,
              FPS = 120;

constexpr Vector2 ORIGIN = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
constexpr float FIXED_TIMESTEP = 1.0f / 60.0f;

AppStatus gAppStatus = RUNNING;
float gPreviousTicks = 0.0f;
float gTimeAccumulator = 0.0f;
int gLives = 3;

Menu *gMenu = nullptr;
LevelA *gLevelA = nullptr;
LevelB *gLevelB = nullptr;
LevelC *gLevelC = nullptr;
LoseScene *gLoseScene = nullptr;
WinScene *gWinScene = nullptr;
Scene *gCurrentScene = nullptr;
Camera2D gCamera = {};
Music gMusic = {};
Sound gStartSound = {};
Sound gJumpSound = {};
Sound gHitSound = {};

bool isLevel()
{
    return gCurrentScene == gLevelA ||
           gCurrentScene == gLevelB ||
           gCurrentScene == gLevelC;
}

void switchToScene(Scene *scene)
{
    if (gCurrentScene != nullptr) gCurrentScene->shutdown();
    gCurrentScene = scene;
    gCurrentScene->initialise();
}

void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Bamboo Trial");
    InitAudioDevice();

    gMusic = LoadMusicStream("assets/music/lotus_pond_loop.wav");
    SetMusicVolume(gMusic, 0.3f);
    PlayMusicStream(gMusic);

    gStartSound = LoadSound("assets/sfx/item_pickup.ogg");
    gJumpSound = LoadSound("assets/sfx/jump_cloth.ogg");
    gHitSound = LoadSound("assets/sfx/sword_slash.ogg");

    gMenu = new Menu(ORIGIN, "#10251D");
    gLevelA = new LevelA(ORIGIN, "#305B47");
    gLevelB = new LevelB(ORIGIN, "#214233");
    gLevelC = new LevelC(ORIGIN, "#152428");
    gLoseScene = new LoseScene(ORIGIN, "#10251D");
    gWinScene = new WinScene(ORIGIN, "#10251D");
    switchToScene(gMenu);
    gCamera.target = ORIGIN;
    gCamera.offset = ORIGIN;
    gCamera.rotation = 0.0f;
    gCamera.zoom = 1.0f;

    SetTargetFPS(FPS);
    gPreviousTicks = static_cast<float>(GetTime());
}

void processInput()
{
    if (IsKeyPressed(KEY_Q) || WindowShouldClose())
    {
        gAppStatus = TERMINATED;
        return;
    }

    if (gCurrentScene == gMenu || isLevel())
    {
        if (IsKeyPressed(KEY_ONE))
        {
            switchToScene(gLevelA);
            return;
        }
        if (IsKeyPressed(KEY_TWO))
        {
            switchToScene(gLevelB);
            return;
        }
        if (IsKeyPressed(KEY_THREE))
        {
            switchToScene(gLevelC);
            return;
        }
    }

    if (!isLevel()) return;

    Entity *player = gCurrentScene->getState().xochitl;
    player->resetMovement();

    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
        player->moveLeft();
    else if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
        player->moveRight();

    if (IsKeyPressed(KEY_SPACE) && player->isCollidingBottom())
    {
        player->jump();
        PlaySound(gJumpSound);
    }
}

void update()
{
    UpdateMusicStream(gMusic);

    float ticks = static_cast<float>(GetTime());
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks = ticks;

    deltaTime += gTimeAccumulator;

    while (deltaTime >= FIXED_TIMESTEP && gAppStatus == RUNNING)
    {
        gCurrentScene->update(FIXED_TIMESTEP);
        deltaTime -= FIXED_TIMESTEP;

        if (isLevel() &&
            (gCurrentScene->playerTouchesEnemy() ||
             gCurrentScene->getState().xochitl->getPosition().y > 700.0f))
        {
            gLives--;
            PlaySound(gHitSound);

            if (gLives <= 0)
                switchToScene(gLoseScene);
            else
                switchToScene(gCurrentScene);
        }

        SceneID nextScene = gCurrentScene->getState().nextSceneID;
        if (nextScene == LEVEL_A)
        {
            if (gCurrentScene == gMenu) PlaySound(gStartSound);
            switchToScene(gLevelA);
        }
        else if (nextScene == LEVEL_B)
            switchToScene(gLevelB);
        else if (nextScene == LEVEL_C)
            switchToScene(gLevelC);
        else if (nextScene == WIN_SCENE)
            switchToScene(gWinScene);
        else if (nextScene == MENU_SCENE)
        {
            switchToScene(gMenu);
            gLives = 3;
            gCamera.target = ORIGIN;
        }

        if (isLevel())
        {
            gCamera.target.x =
                gCurrentScene->getState().xochitl->getPosition().x;

            float leftEdge =
                gCurrentScene->getState().map->getLeftBoundary() + ORIGIN.x;
            float rightEdge =
                gCurrentScene->getState().map->getRightBoundary() - ORIGIN.x;

            if (gCamera.target.x < leftEdge)
                gCamera.target.x = leftEdge;

            if (gCamera.target.x > rightEdge)
                gCamera.target.x = rightEdge;
        }
    }
    gTimeAccumulator = deltaTime;
}

void render()
{
    BeginDrawing();

    if (isLevel())
    {
        BeginMode2D(gCamera);
        gCurrentScene->render();
        EndMode2D();

        DrawRectangle(0, 0, SCREEN_WIDTH, 45, ColorFromHex("#10251D"));
        std::string livesText = "LIVES: " + std::to_string(gLives);
        DrawText(livesText.c_str(), 20, 20, 20, RAYWHITE);

        const char *levelText = "LEVEL 1: BAMBOO";
        if (gCurrentScene == gLevelB) levelText = "LEVEL 2: MOUNTAIN PATH";
        if (gCurrentScene == gLevelC) levelText = "LEVEL 3: TEMPLE";
        DrawText(levelText, 330, 20, 20, RAYWHITE);
    }
    else
    {
        gCurrentScene->render();
    }

    EndDrawing();
}

void shutdown()
{
    gCurrentScene->shutdown();
    delete gMenu;
    delete gLevelA;
    delete gLevelB;
    delete gLevelC;
    delete gLoseScene;
    delete gWinScene;

    UnloadMusicStream(gMusic);
    UnloadSound(gStartSound);
    UnloadSound(gJumpSound);
    UnloadSound(gHitSound);
    CloseAudioDevice();
    CloseWindow();
}

int main()
{
    initialise();

    while (gAppStatus == RUNNING)
    {
        processInput();
        if (gAppStatus == TERMINATED) break;

        update();
        render();
    }

    shutdown();
    return 0;
}
