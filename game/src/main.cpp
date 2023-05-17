#include "rlImGui.h"
#include "raylib.h"
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sunshine");
    SetTargetFPS(60);
    Vector2 circlePos = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
    float circleSpeed = 5.0f;


    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_UP))
        {
            circlePos.y -= circleSpeed;
        }

        if (IsKeyPressed(KEY_DOWN))
        {
            circlePos.y += circleSpeed;
        }

        if (IsKeyPressed(KEY_LEFT))
        {
            circlePos.x -= circleSpeed;
        }

        if (IsKeyPressed(KEY_RIGHT))
        {
            circlePos.x += circleSpeed;
        }
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Hello World!", 16, 9, 20, RED);
        DrawLine(20, 20, 120, 120, BLUE);
        DrawRectangle(560, 600, 150, 70, GREEN);
        DrawCircleV(circlePos, 70, BLUE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}