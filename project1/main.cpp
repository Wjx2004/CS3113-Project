/**

* Author: Junxiao Wang

* Assignment: Pong Clone

* Date due: 10/05/2026

* I pledge that I have completed this assignment without

* collaborating with anyone else, in conformance with the

* NYU School of Engineering Policies and Procedures on

* Academic Misconduct.

**/
#include "CS3113/cs3113.h"
#include <cmath>


constexpr int SCREEN_WIDTH  = 1200,
              SCREEN_HEIGHT = 675,
              FPS           = 60;
constexpr Vector2 START_PLACE = {
    SCREEN_WIDTH - 100.0f,
    SCREEN_HEIGHT / 2.0f
};

constexpr Vector2 HORSE_SIZE = {180.0f,99.0f};
constexpr Vector2 TIGER_SIZE = {192.0f,192.0f};
constexpr Vector2 BULL_SIZE = {192.0f,192.0f};

constexpr float ANIMAL_SPEED = 45.0f;

constexpr float HORSE_GROWTH = 5.0f;
constexpr float HORSE_BOUNCE_SIZE = 5.0f;
constexpr float HORSE_BOUNCE_SPEED = 0.8f;

constexpr float TIGER_UP_SPEED = 8.0f;
constexpr float TIGER_WAVE_SIZE = 15.0f;
constexpr float TIGER_WAVE_SPEED = 1.2f;

constexpr float BULL_DOWN_SPEED = 8.0f;
constexpr float BULL_SWING_SIZE = 8.0f;
constexpr float BULL_SHRINK = 3.0f;
constexpr float BULL_SWING_SPEED= 1.2f;


constexpr float BACKGROUND_COLOR_TIME = 2.0f;
constexpr float BACKGROUND_CYCLE_TIME = 8.0f;

constexpr char GREEN_ONE[] = "#cbf9d2";
constexpr char GREEN_TWO[] = "#4ffc69";
constexpr char GREEN_THREE[] = "#03bc1f";
constexpr char GREEN_FOUR[] = "#027714";
constexpr char HORSE_FP[] = "assets/animals/horse_run.png";
constexpr char TIGER_FP[] = "assets/animals/tiger.png";
constexpr char BULL_FP[] = "assets/animals/bull.png";

AppStatus appStatus = RUNNING;
Texture2D horseTexture;
Texture2D tigerTexture;
Texture2D bullTexture;
Vector2 horsePosition = START_PLACE;
Vector2 tigerPosition = START_PLACE;
Vector2 bullPosition = START_PLACE;
Vector2 horseSize = HORSE_SIZE;
Vector2 tigerSize = TIGER_SIZE;
Vector2 bullSize = BULL_SIZE;
float bullRotation = 0.0f;
float movementTime = 0.0f;
float previousTime = 0.0f;
float backgroundTime = 0.0f;
Color backgroundColor;

void initialise();
void processInput();
void update();
void render();
void shutdown();

void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Animal Stampede");
    horseTexture = LoadTexture(HORSE_FP);
    tigerTexture = LoadTexture(TIGER_FP);
    bullTexture = LoadTexture(BULL_FP);
    backgroundColor = ColorFromHex(GREEN_ONE);
    SetTargetFPS(FPS);
}

void processInput()
{
    if (WindowShouldClose()) appStatus = TERMINATED;
}

void update()
{
    float currentTime = static_cast<float>(GetTime());
    float daltaTime = currentTime - previousTime;
    previousTime = currentTime;
    movementTime += daltaTime;
    backgroundTime += daltaTime;
    if (backgroundTime >= BACKGROUND_CYCLE_TIME)
    {
        backgroundTime = 0.0f;
    }

    if (backgroundTime < BACKGROUND_COLOR_TIME)
    {
        backgroundColor = ColorFromHex(GREEN_ONE);
    }
    else if (backgroundTime < BACKGROUND_COLOR_TIME * 2.0f)
    {
        backgroundColor = ColorFromHex(GREEN_TWO);
    }
    else if (backgroundTime < BACKGROUND_COLOR_TIME * 3.0f)
    {
        backgroundColor = ColorFromHex(GREEN_THREE);
    }
    else
    {
        backgroundColor = ColorFromHex(GREEN_FOUR);
    }

    horsePosition.x -= ANIMAL_SPEED * daltaTime;
    horsePosition.y = START_PLACE.y + (HORSE_BOUNCE_SIZE * sin(movementTime * HORSE_BOUNCE_SPEED));
    horseSize.x += HORSE_GROWTH * daltaTime;
    horseSize.y = (horseSize.x * HORSE_SIZE.y) / HORSE_SIZE.x;

    tigerPosition.y = START_PLACE.y - TIGER_UP_SPEED * movementTime + TIGER_WAVE_SIZE * sin(movementTime * TIGER_WAVE_SPEED);
    tigerPosition.x = horsePosition.x;


    bullPosition.x = horsePosition.x ;
    bullPosition.y = START_PLACE.y + BULL_DOWN_SPEED * movementTime;
    bullSize.x -= BULL_SHRINK * daltaTime;
    bullSize.y = bullSize.x;
    bullRotation = BULL_SWING_SIZE * sin(movementTime * BULL_SWING_SPEED);

    if (horsePosition.x < -horseSize.x / 2.0f)
    {
        horsePosition = START_PLACE;
        tigerPosition = START_PLACE;
        bullPosition = START_PLACE;
        horseSize = HORSE_SIZE;
        bullSize = BULL_SIZE;
        bullRotation = 0.0f;
        movementTime = 0.0f;

    }

}
void render()
{
    BeginDrawing();
    ClearBackground(backgroundColor);
    Rectangle tigerTextureArea = {64.0f,192.0f,64.0f,64.0f};
    Rectangle tigerDestinationArea = {tigerPosition.x,tigerPosition.y,tigerSize.x,tigerSize.y};
    Vector2 tigerOrigin = {tigerSize.x/2.0f,tigerSize.y/2.0f};
    DrawTexturePro(tigerTexture,tigerTextureArea,tigerDestinationArea,tigerOrigin,0.0f,WHITE);
    Rectangle bullTextureArea = {64.0f,192.0f,64.0f,64.0f};
    Rectangle bullDestinationArea = {bullPosition.x,bullPosition.y,bullSize.x,bullSize.y};
    Vector2 bullOrigin = {bullSize.x / 2.0f,bullSize.y /2.0f};
    DrawTexturePro(bullTexture,bullTextureArea,bullDestinationArea,bullOrigin,bullRotation,WHITE);
    Rectangle horseTextureArea = {0.0f,0.0f,60.0f,33.0f};
    Rectangle horseDestinationArea = {horsePosition.x,horsePosition.y,horseSize.x,horseSize.y};
    Vector2 horseOrigin = {horseSize.x/2.0f,horseSize.y/2.0f};
    DrawTexturePro(horseTexture,horseTextureArea,horseDestinationArea,horseOrigin,0.0f,WHITE);
    EndDrawing();
}
void shutdown()
{
    UnloadTexture(horseTexture);
    UnloadTexture(tigerTexture);
    UnloadTexture(bullTexture);
    CloseWindow(); // Close window and OpenGL context
}

int main(void)
{
    initialise();

    while (appStatus == RUNNING)
    {
        processInput();
        update();
        render();
    }

    shutdown();

    return 0;
}

