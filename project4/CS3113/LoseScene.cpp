/**
 * Author: Junxiao Wang
 * Assignment: Rise of the AI
 * Date due: 11/16/2026
 * I pledge that I have completed this assignment without
 * collaborating with anyone else, in conformance with the
 * NYU School of Engineering Policies and Procedures on
 * Academic Misconduct.
 */

#include "LoseScene.h"

LoseScene::LoseScene(Vector2 origin, const char *bgHexCode)
    : Scene{ origin, bgHexCode }
{}

void LoseScene::initialise(){mGameState.nextSceneID = NO_SCENE;}

void LoseScene::update(float deltaTime)
{
    (void) deltaTime;
    if (IsKeyDown(KEY_R))
        mGameState.nextSceneID = MENU_SCENE;
}

void LoseScene::render()
{
    ClearBackground(ColorFromHex(mBGColourHexCode));
    DrawText("YOU LOSE", 360, 220, 50, RAYWHITE);
    DrawText("Press R to return to menu", 330, 310, 25, RAYWHITE);
}

void LoseScene::shutdown()
{
}
