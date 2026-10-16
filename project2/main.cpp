/**

* Author: Junxiao Wang

* Assignment: Pong Clone

* Date due: 10/16/2026

* I pledge that I have completed this assignment without

* collaborating with anyone else, in conformance with the

* NYU School of Engineering Policies and Procedures on

* Academic Misconduct.

**/
#include "CS3113/cs3113.h"
#include <math.h>

constexpr int SCREEN_WIDTH  = 1600 / 2,
              SCREEN_HEIGHT = 900 / 2,
              FPS = 60;
constexpr int FOOT_SPEED = 250;
constexpr int BALL_SPEED = 240;
constexpr int MAX_BALLS = 5;
constexpr int WINNING_SCORE = 5;
// This green background is kept as a fallback and does not affect
// normal rendering because the soccer field texture covers it.
constexpr char BACKGROUND_IMAGE_PATH[] = "assets/SoccerField.png";
constexpr char BACKGROUND_COLOR[] = "#4fff69";
constexpr char LEFT_WIN_IMAGE_PATH[] = "assets/left-wins.png";
constexpr char RIGHT_WIN_IMAGE_PATH[] = "assets/right-wins.png";
constexpr char FOOT_IMAGE_PATH[] = "assets/foot-png.png";
constexpr char BALL_IMAGE_PATH[] = "assets/SoccerBall.png";

AppStatus gAppStatus = RUNNING;
bool gPlayerSingleMode = false;
float gPreviousFrameTime = 0.0f;
int gNumberOfBalls = 1;
int gLeftPlayerScore = 0;
int gRightPlayerScore = 0;
int gWinningPlayer = 0;
bool gGameOver = false;

Vector2 gBackgroundPosition = {SCREEN_WIDTH / 2.0f,SCREEN_HEIGHT / 2.0f};
Vector2 gBackgroundSize = {static_cast<float>(SCREEN_WIDTH),static_cast<float>(SCREEN_HEIGHT)};
Vector2 gLeftFootPosition = { 70.0f, SCREEN_HEIGHT / 2.0f};
Vector2 gRightFootPosition = {SCREEN_WIDTH - 70.0f, SCREEN_HEIGHT / 2.0f };
Vector2 gLeftFootMovement = { 0.0f, 0.0f };
Vector2 gRightFootMovement = { 0.0f, 0.0f };
Vector2 gFootSize = {240.0f, 160.0f};
Vector2 gFootCollisionSize = { 45.0f, 145.0f };
Vector2 gBallPositions[MAX_BALLS] = {{SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f},{SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f - 60.0f},{SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f + 60.0f},{SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f - 120.0f},{SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f + 120.0f}};
Vector2 gBallMovements[MAX_BALLS] = {{ 0.9f,  0.3f},{-0.9f,  0.3f},{ 0.9f, -0.3f},{-0.8f, -0.5f},{ 0.8f,  0.5f}};
Vector2 gBallSize = { 40.0f, 40.0f };
Vector2 gScoreBallSize = {24.0f, 24.0f};
Vector2 gWinMessagePosition = {SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f};
Vector2 gWinMessageSize = {420.0f, 168.0f};
Texture2D gBackgroundTexture;
Texture2D gFootTexture;
Texture2D gBallTexture;
Texture2D gLeftWinTexture;
Texture2D gRightWinTexture;
void initialise();
void processInput();
void update();
void render();
void shutdown();
void renderScore();

bool isColliding(
    const Vector2 *positionAFoot,
    const Vector2 *sizeA,
    const Vector2 *positionBFoot,
    const Vector2 *sizeB
);
bool isColliding(
    const Vector2 *positionAFoot,
    const Vector2 *sizeA,
    const Vector2 *positionBFoot,
    const Vector2 *sizeB
)
{
    float XDistance = fabs(positionAFoot->x - positionBFoot->x) - (sizeA->x + sizeB->x) /2.0f;
    float YDistance = fabs(positionAFoot->y - positionBFoot->y) - (sizeA->y + sizeB->y) /2.0f;
    return XDistance < 0.0f && YDistance < 0.0f;
}

void renderObject(
    const Texture2D *texture,
    const Vector2 *position,
    const Vector2 *size
);

void renderScore()
{
    for (int i = 0; i < gLeftPlayerScore; i++)
    {
        Vector2 scoreBallPosition = {30.0f + i * 30.0f,32.0f};
        renderObject(
            &gBallTexture,
            &scoreBallPosition,
            &gScoreBallSize
        );
    }
    for (int i = 0; i < gRightPlayerScore; i++)
    {
        Vector2 scoreBallPosition = {SCREEN_WIDTH - 30.0f - i * 30.0f,32.0f};
        renderObject(
            &gBallTexture,
            &scoreBallPosition,
            &gScoreBallSize
        );
    }
}

void updateFoot(
    Vector2 *position,
    const Vector2 *movement,
    float deltaTime
);
void updateBall(Vector2 *gBallPosition, Vector2 *gBallMovement,float deltaTime);
void renderObject(
    const Texture2D *texture,
    const Vector2 *position,
    const Vector2 *size
)
{
    Rectangle textureRectangle = {
        0.0f,
        0.0f,
        static_cast<float>(texture->width),
        static_cast<float>(texture->height)
    };
    Rectangle destinationArea = {
        position->x,
        position->y,
        size->x,
        size->y
    };
    Vector2 originOffset = {
        size->x / 2.0f,
        size->y / 2.0f
    };
    DrawTexturePro(
        *texture,
        textureRectangle,destinationArea,originOffset,
        0.0f,WHITE
    );
}


void updateBall(Vector2 *gBallPosition,Vector2 *gBallMovement,float deltaTime)
{
    gBallPosition->x += BALL_SPEED * gBallMovement->x * deltaTime;
    gBallPosition->y += BALL_SPEED * gBallMovement->y * deltaTime;

    float ballHalfWidth = gBallSize.x / 2.0f;
    float ballHalfHeight = gBallSize.y / 2.0f;

    if (gBallPosition->y < ballHalfHeight)
    {gBallPosition->y = ballHalfHeight; gBallMovement->y = -gBallMovement->y;}

    if (gBallPosition->y > SCREEN_HEIGHT - ballHalfHeight)
    {gBallPosition->y = SCREEN_HEIGHT - ballHalfHeight; gBallMovement->y = -gBallMovement->y;}

    if (gBallMovement->x < 0.0f && isColliding(gBallPosition, &gBallSize, &gLeftFootPosition, &gFootCollisionSize))
    {
        gBallPosition->x = gLeftFootPosition.x + gFootCollisionSize.x / 2.0f + gBallSize.x / 2.0f;
        gBallMovement->x = -gBallMovement->x;
    }

    if (gBallMovement->x > 0.0f && isColliding(gBallPosition, &gBallSize, &gRightFootPosition, &gFootCollisionSize))
    {
        gBallPosition->x = gRightFootPosition.x - gFootCollisionSize.x / 2.0f - gBallSize.x / 2.0f;
        gBallMovement->x = -gBallMovement->x;
    }

    if (gBallPosition->x + ballHalfWidth < 0.0f)
    {
        gRightPlayerScore++;
        gBallPosition->x = SCREEN_WIDTH / 2.0f;
        gBallPosition->y = SCREEN_HEIGHT / 2.0f;
        gBallMovement->x = 0.9f;
        if(gRightPlayerScore >= WINNING_SCORE)
        {
            gWinningPlayer = 2;
            gGameOver = true;
        }
    }

    if (gBallPosition->x - ballHalfWidth > SCREEN_WIDTH)
    {
        gLeftPlayerScore++;
        gBallPosition->x = SCREEN_WIDTH / 2.0f;
        gBallPosition->y = SCREEN_HEIGHT / 2.0f;
        gBallMovement->x = -0.9f;
        if(gLeftPlayerScore >= WINNING_SCORE)
        {
            gWinningPlayer = 1;
            gGameOver = true;
        }
    }
}
void updateFoot(
    Vector2 *position,
    const Vector2 *movement,
    float deltaTime
)
{
    position->y += FOOT_SPEED * movement->y * deltaTime;
    float footHalfHeight = gFootSize.y / 2.0f;
    if (position->y - footHalfHeight < 0.0f){position->y = footHalfHeight;}
    if (position->y + footHalfHeight > SCREEN_HEIGHT){position->y = SCREEN_HEIGHT - footHalfHeight;}
}

void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Foot Pong");
    gBackgroundTexture = LoadTexture(BACKGROUND_IMAGE_PATH);
    gFootTexture = LoadTexture(FOOT_IMAGE_PATH);
    gBallTexture = LoadTexture(BALL_IMAGE_PATH);
    gLeftWinTexture = LoadTexture(LEFT_WIN_IMAGE_PATH);
    gRightWinTexture = LoadTexture(RIGHT_WIN_IMAGE_PATH);
    SetTargetFPS(FPS);
}

void processInput()
{   
    gLeftFootMovement = {0.0f, 0.0f};
    gRightFootMovement = {0.0f, 0.0f};
    if (IsKeyPressed(KEY_Q) || WindowShouldClose())
    {
        gAppStatus = TERMINATED;
        return;
    }
    if (gGameOver) return;
    if (IsKeyPressed(KEY_T)) gPlayerSingleMode = !gPlayerSingleMode;

    if (IsKeyPressed(KEY_ONE)) gNumberOfBalls = 1;
    if (IsKeyPressed(KEY_TWO)) gNumberOfBalls = 2;
    if (IsKeyPressed(KEY_THREE)) gNumberOfBalls = 3;
    if (IsKeyPressed(KEY_FOUR)) gNumberOfBalls = 4;
    if (IsKeyPressed(KEY_FIVE)) gNumberOfBalls = 5;

    if (IsKeyDown(KEY_W)) gLeftFootMovement.y = -1.0f;
    else if (IsKeyDown(KEY_S)) gLeftFootMovement.y = 1.0f;
    if (!gPlayerSingleMode)
    {
        if (IsKeyDown(KEY_UP))
        {gRightFootMovement.y = -1.0f;}
        else if (IsKeyDown(KEY_DOWN))
        {gRightFootMovement.y = 1.0f;}
    }
}
void update()
{   
    if (gGameOver) return;
    float currentFrameTime = static_cast<float>(GetTime());
    float deltaTime = currentFrameTime - gPreviousFrameTime;
    gPreviousFrameTime = currentFrameTime;

    if (gPlayerSingleMode)
    {
        if (gBallPositions[0].y < gRightFootPosition.y - 10.0f)
        {gRightFootMovement.y = -1.0f;}
        else if (gBallPositions[0].y > gRightFootPosition.y + 10.0f)
        {gRightFootMovement.y = 1.0f;}
        else
        {gRightFootMovement.y = 0.0f;}
    }
    updateFoot(
        &gLeftFootPosition,
        &gLeftFootMovement,
        deltaTime
    );

    updateFoot(
        &gRightFootPosition,
        &gRightFootMovement,
        deltaTime
    );
    for (int i = 0; i < gNumberOfBalls && !gGameOver; i++)
    {
        updateBall(
            &gBallPositions[i],
            &gBallMovements[i],
            deltaTime
        );
    }
}
void render()
{
    BeginDrawing();
    ClearBackground(ColorFromHex(BACKGROUND_COLOR));
    renderObject(
        &gBackgroundTexture,
        &gBackgroundPosition,
        &gBackgroundSize
    );

    renderObject(
        &gFootTexture,
        &gLeftFootPosition,
        &gFootSize
    );
    renderObject(
        &gFootTexture,
        &gRightFootPosition,
        &gFootSize
    );

    for (int i = 0; i < gNumberOfBalls; i++)
    {
        renderObject(
            &gBallTexture,
            &gBallPositions[i],
            &gBallSize
        );
    }
    renderScore();
    if (gGameOver)
    {
        if (gWinningPlayer == 1)
        {
            renderObject(
                &gLeftWinTexture,
                &gWinMessagePosition,
                &gWinMessageSize
            );
        }
        else if (gWinningPlayer == 2)
        {
            renderObject(
                &gRightWinTexture,
                &gWinMessagePosition,
                &gWinMessageSize
            );
        }
    }
    EndDrawing();
}
void shutdown()
{   
    UnloadTexture(gBackgroundTexture);
    UnloadTexture(gFootTexture);
    UnloadTexture(gBallTexture);
    UnloadTexture(gLeftWinTexture);
    UnloadTexture(gRightWinTexture);
    CloseWindow();
}
int main(void)
{
    initialise();

    while (gAppStatus == RUNNING)
    {
        processInput();
        update();
        render();
    }

    shutdown();

    return 0;
}

















