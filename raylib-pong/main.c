#include <stdio.h>
#include "raylib.h"

typedef struct {
    float x;
    float y;
    int width;
    int height;
    int middle;
    int speedX;
    int speedY;
    Color color;
    int score;
    Rectangle rec;
} Entity;


int main(void)
{

    // Variables
    const int screenWidth = 1280;
    const int screenHeight = 720;

    // Create player
    Entity player;
    player.x = 10;
    player.y = screenHeight / 2;
    player.width = 10;
    player.height = 100;
    player.middle = player.y - player.height;
    player.speedX = 0;
    player.speedY = 300;
    player.color = (Color){ 255, 255, 255, 255 };
    player.score = 0;
    player.rec = (Rectangle) {player.x, player.y, player.width, player.height};

    // Create ai
    Entity ai;
    ai.x = screenWidth - 20;
    ai.y = screenHeight / 2;
    ai.width = 10;
    ai.height = 100;
    ai.middle = ai.y - ai.height;
    ai.speedX = 0;
    ai.speedY = 200;
    ai.color = (Color){ 255, 255, 255, 255 };
    ai.score = 0;
    ai.rec = (Rectangle) {ai.x, ai.y, ai.width, ai.height};

    // Create ball
    Entity ball;
    ball.x = screenWidth / 2 - 10;
    ball.y = screenHeight / 2;
    ball.width = 20;
    ball.height = 20;
    ball.middle = ball.y - ball.height;
    ball.speedX = -400;
    ball.speedY = 0;
    ball.color = (Color){ 255, 255, 255, 255 };
    ball.score = 0;
    ball.rec = (Rectangle) {ball.x, ball.y, ball.width, ball.height};

    bool running = false;
    char playerScoreText[20];
    char aiScoreText[20];

    int collisionPos;

    // Initialize window
    InitWindow(screenWidth, screenHeight, "Pong by mouse");

    // Set FPS
    SetTargetFPS(60);

    // Game loop
    while (!WindowShouldClose())
    {

        // Calculate delta time
        float dt = GetFrameTime();

        // Convert scores from int to strings for drawing to screen
        snprintf(playerScoreText, sizeof(playerScoreText), "%d", player.score);
        snprintf(aiScoreText, sizeof(aiScoreText), "%d", ai.score);

        // Update player
        // Check input
        if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) player.y += player.speedY * dt;
        if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) player.y += -player.speedY * dt;
        if (IsKeyDown(KEY_SPACE)) running = true;
        // Check player boundaries
        if (player.y < 0) player.y = 0;
        if (player.y > screenHeight - player.height) player.y = screenHeight - player.height; 
        // Update player rectangle
        player.rec = (Rectangle) {player.x, player.y, player.width, player.height};

        // Update ai
        // Update ai rectangle
        ai.rec = (Rectangle) {ai.x, ai.y, ai.width, ai.height};
        // Move ai 
        if ((ball.y + ball.height / 2) > (ai.y + ai.height / 2) && ball.speedX > 0)
        {
            ai.y += ai.speedY * dt;
        }
        if ((ball.y + ball.height / 2) < (ai.y + ai.height / 2) && ball.speedX > 0)
        {
            ai.y -= ai.speedY * dt;
        }
        // Check ai boundaries
        if (ai.y < 0) ai.y = 0;
        if (ai.y > screenHeight - ai.height) ai.y = screenHeight - ai.height;

        // Update ball
        // Start updating once running true
        if (running == true)
        {
            // Update position and check boundary
            ball.x += ball.speedX * dt;
            ball.y += ball.speedY * dt;
            // If ball hits top
            if (ball.y < 0)
            {
                ball.y = 0;
                ball.speedY *= -1;
            }
            // If ball hits bottom
            if (ball.y > screenHeight - ball.height)
            {
                ball.y = screenHeight - ball.height;
                ball.speedY *= -1;
            }
            // If ball goes past player
            if (ball.x < 0) 
            {
                ai.score += 1;
                ball.x = screenWidth / 2 - 10;
                ball.y = screenHeight / 2;
                ball.speedX *= -1;
                ball.speedY = 0;
                running = false;
            }
            // If ball goes past ai
            if (ball.x > screenWidth) 
            {
                player.score += 1;
                ball.x = screenWidth / 2 - 10;
                ball.y = screenHeight / 2;
                ball.speedX *= -1;
                ball.speedY = 0;
                running = false;
            }

            // Update ball rectangle
            ball.rec = (Rectangle) {ball.x, ball.y, ball.width, ball.height};

            // Check if ball collides with player or ai
            if (CheckCollisionRecs(ball.rec, player.rec) && ball.speedX < 0)
            {
                // Get point of collision. Change speedY based on hit location
                collisionPos = (ball.y + ball.height / 2) - (player.y + player.height / 2);
                ball.speedX *= -1;
                ball.speedY = collisionPos * 15;

            }
            if (CheckCollisionRecs(ball.rec, ai.rec) && ball.speedX > 0)
            {
                // Get point of collision. Change speedY based on hit location
                collisionPos = (ball.y + ball.height / 2) - (ai.y + ai.height / 2);
                ball.speedX *= -1;
                ball.speedY = collisionPos * 15;
            }

        }
        

        // Draw
        BeginDrawing();

            // Clear background to overwrite previous frame
            ClearBackground(BLACK);

            // Draw player
            DrawRectangle(player.x, player.y, player.width, player.height, player.color); 
            // Draw ai 
            DrawRectangle(ai.x, ai.y, ai.width, ai.height, ai.color); 
            // Draw ball
            DrawRectangle(ball.x, ball.y, ball.width, ball.height, ball.color);

            // Draw scores
            DrawText(playerScoreText, 10, 10, 40, WHITE);
            DrawText(aiScoreText, screenWidth - 30, 10, 40, WHITE);

            // Draw center line
            DrawLine(screenWidth / 2, 0, screenWidth / 2, screenHeight, WHITE);

            // Draw message if not running
            if (running == false) DrawText("Press Spacebar to start!", 350, 100, 40, WHITE);

        EndDrawing();

    }

    // Close window and cleanup
    CloseWindow();

    return 0;
}