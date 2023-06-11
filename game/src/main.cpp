#include <iostream>
#include <vector>
#include "raylib.h"
#include <raymath.h>

struct Circle
{
    Vector2 center;
    float radius;
};

class Rigidbody
{
public:
    Vector2 position;
    Vector2 velocity;
};

class Agent
{
public:
    Rigidbody rigidbody;
    Circle circle;
    float maxSpeed;
    float maxAcceleration;
    Vector2 leftWhisker;
    Vector2 rightWhisker;

    void obstacleAvoidance(const Circle& obstacle, float avoidanceForce)
    {
        Vector2 ahead = Vector2Add(rigidbody.position, Vector2Scale(Vector2Normalize(rigidbody.velocity), 50.0f));
        Vector2 ahead2 = Vector2Add(rigidbody.position, Vector2Scale(Vector2Normalize(rigidbody.velocity), 25.0f));

        Vector2 avoidance = { 0, 0 };
        bool collisionDetected = false;

        if (CheckCollisionCircles(obstacle.center, obstacle.radius, circle.center, circle.radius))
        {
            avoidance = Vector2Subtract(rigidbody.position, obstacle.center);
            collisionDetected = true;
        }
        else if (CheckCollisionCircleRec(obstacle.center, obstacle.radius, { ahead.x, ahead.y, circle.radius, circle.radius }))
        {
            avoidance = Vector2Subtract(rigidbody.position, obstacle.center);
            collisionDetected = true;
        }
        else if (CheckCollisionCircleRec(obstacle.center, obstacle.radius, { ahead2.x, ahead2.y, circle.radius, circle.radius }))
        {
            avoidance = Vector2Subtract(rigidbody.position, obstacle.center);
            collisionDetected = true;
        }

        if (collisionDetected)
        {
            avoidance = Vector2Normalize(avoidance);
            avoidance = Vector2Scale(avoidance, avoidanceForce);

            rigidbody.velocity = Vector2Add(rigidbody.velocity, avoidance);
            float speed = Vector2Length(rigidbody.velocity);
            if (speed > maxSpeed)
            {
                rigidbody.velocity = Vector2Scale(rigidbody.velocity, maxSpeed / speed);
            }
        }
    }
};

int main(void)
{
    const int SCREEN_WIDTH = 1280;
    const int SCREEN_HEIGHT = 720;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Agent Avoidance");
    SetTargetFPS(60);

    Agent agent1;
    agent1.rigidbody.position = { SCREEN_WIDTH / 2.0f - 100, SCREEN_HEIGHT / 2.0f };
    agent1.rigidbody.velocity = { 2.0f, 2.0f };
    agent1.maxSpeed = 20.0f;
    agent1.maxAcceleration = 1.0f;
    agent1.circle = { agent1.rigidbody.position, 20.0f };
    agent1.leftWhisker = Vector2Rotate(Vector2Normalize(agent1.rigidbody.velocity), -45.0f);
    agent1.rightWhisker = Vector2Rotate(Vector2Normalize(agent1.rigidbody.velocity), 45.0f);

    Agent agent2;
    agent2.rigidbody.position = { SCREEN_WIDTH / 2.0f + 100, SCREEN_HEIGHT / 2.0f };
    agent2.rigidbody.velocity = { -2.0f, -2.0f };
    agent2.maxSpeed = 20.0f;
    agent2.maxAcceleration = 1.0f;
    agent2.circle = { agent2.rigidbody.position, 20.0f };
    agent2.leftWhisker = Vector2Rotate(Vector2Normalize(agent2.rigidbody.velocity), -45.0f);
    agent2.rightWhisker = Vector2Rotate(Vector2Normalize(agent2.rigidbody.velocity), 45.0f);

    std::vector<Circle> obstacles;

    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            Vector2 mousePos = GetMousePosition();
            float obstacleRadius = 10.0f;
            Circle obstacle = { mousePos, obstacleRadius };
            obstacles.push_back(obstacle);
        }

        for (const Circle& obstacle : obstacles)
        {
            agent1.obstacleAvoidance(obstacle, 0.5f);
            agent2.obstacleAvoidance(obstacle, 0.5f);
        }

        agent1.rigidbody.position.x += agent1.rigidbody.velocity.x * deltaTime;
        agent1.rigidbody.position.y += agent1.rigidbody.velocity.y * deltaTime;
        agent1.circle.center = agent1.rigidbody.position;

        agent2.rigidbody.position.x += agent2.rigidbody.velocity.x * deltaTime;
        agent2.rigidbody.position.y += agent2.rigidbody.velocity.y * deltaTime;
        agent2.circle.center = agent2.rigidbody.position;

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawCircleV(agent1.circle.center, agent1.circle.radius, SKYBLUE);
        DrawCircleV(agent2.circle.center, agent2.circle.radius, ORANGE);

        DrawLine(agent1.rigidbody.position.x, agent1.rigidbody.position.y,
            agent1.rigidbody.position.x + agent1.leftWhisker.x * 60,
            agent1.rigidbody.position.y + agent1.leftWhisker.y * 60, BLACK);

        DrawLine(agent1.rigidbody.position.x, agent1.rigidbody.position.y,
            agent1.rigidbody.position.x + agent1.rightWhisker.x * 60,
            agent1.rigidbody.position.y + agent1.rightWhisker.y * 60, VIOLET);

        DrawLine(agent2.rigidbody.position.x, agent2.rigidbody.position.y,
            agent2.rigidbody.position.x + agent2.leftWhisker.x * 60,
            agent2.rigidbody.position.y + agent2.leftWhisker.y * 60, BLACK);

        DrawLine(agent2.rigidbody.position.x, agent2.rigidbody.position.y,
            agent2.rigidbody.position.x + agent2.rightWhisker.x * 60,
            agent2.rigidbody.position.y + agent2.rightWhisker.y * 60, VIOLET);

        for (const Circle& obstacle : obstacles)
        {
            DrawCircleV(obstacle.center, obstacle.radius, GREEN);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
