/**
 * Author: Junxiao Wang
 * Assignment: Rise of the AI
 * Date due: 11/16/2026
 * I pledge that I have completed this assignment without
 * collaborating with anyone else, in conformance with the
 * NYU School of Engineering Policies and Procedures on
 * Academic Misconduct.
 */

#include "Menu.h"

Menu::Menu(Vector2 origin, const char *bgHexCode)
    : Scene{ origin, bgHexCode }
{}

void Menu::initialise()
{
    mGameState.nextSceneID = NO_SCENE;
}
void Menu::update(float deltaTime)
{
    (void) deltaTime;

    if (IsKeyDown(KEY_ENTER))
        mGameState.nextSceneID = LEVEL_A;
}
void Menu::render()
{
    ClearBackground(ColorFromHex(mBGColourHexCode));
    DrawText("BAMBOO TRIAL", 300, 220, 50, RAYWHITE);
    DrawText("Press ENTER to start", 345, 310, 25, RAYWHITE);
    DrawText("A/D or arrows: move    SPACE: jump", 300, 380, 20, RAYWHITE);
    DrawText("1 / 2 / 3 / 4: test levels    Q: quit", 280, 410, 20, RAYWHITE);
}
void Menu::shutdown(){}
