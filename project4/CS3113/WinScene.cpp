/**
 * Author: Junxiao Wang
 * Assignment: Rise of the AI
 * Date due: 11/16/2026
 * I pledge that I have completed this assignment without
 * collaborating with anyone else, in conformance with the
 * NYU School of Engineering Policies and Procedures on
 * Academic Misconduct.
 */

#include "WinScene.h"

WinScene::WinScene(Vector2 origin, const char *bgHexCode)
    : Scene{ origin, bgHexCode }
{}

void WinScene::initialise()
{
    mGameState.nextSceneID = NO_SCENE;
}

void WinScene::update(float deltaTime)
{
    (void) deltaTime;

    if (IsKeyDown(KEY_R))
        mGameState.nextSceneID = MENU_SCENE;
}

void WinScene::render()
{
    ClearBackground(ColorFromHex(mBGColourHexCode));
    DrawText("YOU WIN", 380, 220, 50, RAYWHITE);
    DrawText("The bamboo trial is complete", 310, 290, 25, RAYWHITE);
    DrawText("Press R to return to menu", 330, 340, 25, RAYWHITE);
}

void WinScene::shutdown()
{
}
