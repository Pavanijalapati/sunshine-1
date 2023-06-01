#include "rlImGui.h"
#include"math.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

Vector2 WraparoundScreen(Vector2 position)
{
    Vector2 outPosition =
    {
        fmodf(position.x + SCREEN_WIDTH,SCREEN_WIDTH),
        fmodf(position.y + SCREEN_HEIGHT,SCREEN_HEIGHT)

    };
    return outPosition;

}

int main(void)
{

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "sunshine");

    rlImGuiSetup(true);

    Vector2 position = { 100,100 }; //px
    Vector2 velocity = { 10,0 };  //px/s
    Vector2 acceleration = { 0,50 };   //px/s/s 
    float maxSpeed = 1000;
    float maxAccel = 1000;

    SetTargetFPS(240);

    while (!WindowShouldClose())
    {
        const float dt = GetFrameTime();
        BeginDrawing();
        ClearBackground(RAYWHITE);

        Vector2 a = { 2,2 };
        Vector2 b = { 3,4 };
        Vector2 myvec = a + b;


        rlImGuiBegin();
        ImGui::SliderFloat2("position", &(position.x), 0, SCREEN_WIDTH);
        ImGui::DragFloat2("Velocity", &(velocity.x), -maxSpeed, maxSpeed);
        ImGui::DragFloat2("acceleration", &(acceleration.x), 1, -maxAccel, maxAccel);
        ImGui::LabelText("myvec", "(%f,%f)", myvec.x, myvec.y);

        rlImGuiEnd();

        Vector2 displacement = velocity * dt;
        velocity = velocity + acceleration * dt;
        position = position + displacement;

        position = position + displacement + acceleration * 0.5f * dt * dt;  //acceleration


        DrawCircleV(position, 50, VIOLET);


        DrawLineV(position, position + velocity, RED);
        DrawLineV(position, position + acceleration, GREEN);

        position = WraparoundScreen(position);

        DrawFPS(10, 10);
        EndDrawing();

    }

    rlImGuiShutdown();
    CloseWindow();
    return 0;
}
