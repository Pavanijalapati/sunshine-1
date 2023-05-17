#include "rlImGui.h"
#include "raylib.h"
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sunshine");
    int screenUpperLimit = 40; 
    SetTargetFPS(60);
    

    //Definig the properties of stationary circle
    const int stationaryCircleRadius = 70;
    Vector2 stationaryCirclePosition = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
    Color stationaryCircleColor = BLUE;

    // Defining properties of the circle controlled by mouse cursor
    const int movableCircleRadius = 70;
    Vector2 movableCirclePosition = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
    Color movableCircleColor = BLACK;

    while (!WindowShouldClose())
    {
        movableCirclePosition = GetMousePosition();//function 1:Retrieves the current mouse cursor position.
        //checking for collision
        //Function 2:CheckCollisionCircles :checks if the two circles collide
        bool collision = CheckCollisionCircles(stationaryCirclePosition, stationaryCircleRadius, movableCirclePosition, movableCircleRadius);
        
        //changing the colors of the circles if the collision occurs
        if (collision)
        {
            stationaryCircleColor = RED;
            movableCircleColor = GREEN;
            //Function3:DrawText: Draws the specified text string using the default font at the given position (posX, posY)
            //  with the specified font size and color
            //Function4:GetScreenwidth():Gets current screenwidth
            //function5:MeasureText(const char *text, int fontSize):Measure string width for default font
            DrawText("COLLISION DETECTED!", GetScreenWidth() / 2 - MeasureText("COLLISION DETECTED!", 20) / 2, screenUpperLimit / 2 - 10, 20, BLACK); 
        }
        else
        {
            stationaryCircleColor = BLUE;
            movableCircleColor = BLACK;
            DrawText("NO COLLISION DETECTED YET!", GetScreenWidth() / 2 - MeasureText("NO COLLISION DETECTED YET!", 20) / 2, screenUpperLimit / 2 - 10, 20, BLACK);
        }
        
        //Function6: DrawcircleV: draws a circle with a specified center,radius and color
        DrawCircleV(stationaryCirclePosition, stationaryCircleRadius, stationaryCircleColor);
        DrawCircleV(movableCirclePosition, movableCircleRadius, movableCircleColor);
        //Function7:DrawTriangle: draws a color filled triangle
        DrawTriangle({ 100, 200 }, { 200, 200 }, { 150, 100 }, DARKPURPLE);
        //Function8:DrawRectangleGradientH: Draw a horizontal-gradient-filled rectangle
        DrawRectangleGradientH(300, 170, 180, 130, SKYBLUE, PINK);
        //Function 9:DrawRectangleLines:Draws a rectangle outline
        DrawRectangleLines(400, 320, 80, 60, DARKGRAY);
        //Function10:SetWindowTitle: sets the desired title on window
        SetWindowTitle("Sunshine project");
        //Function11:HideCursor:Hides the cursor in window
        HideCursor();

        BeginDrawing();
        ClearBackground(RAYWHITE);
        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}