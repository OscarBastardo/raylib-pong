#include <raylib.h>
#include <cstdlib>

struct Ball
{
    float x;
    float y;
    float radius;
    float speedX;
    float speedY;

    void draw()
    {
        DrawCircle((int)x, (int)y, (int)radius, WHITE);
    }
};

struct Paddle
{
    float x;
    float y;
    float width;
    float height;
    float speed;

    Rectangle GetRectangle()
    {
        return Rectangle{x - width / 2, y - height / 2, width, height};
    }

    void draw()
    {
        DrawRectangleRec(GetRectangle(), WHITE);
    }
};

struct Player
{
    int score;

    void drawScore(int x)
    {
        DrawText(TextFormat("%i", score), x, 20, 20, WHITE);
    }
};

int main()
{
    InitWindow(800, 600, "Pong");
    SetWindowState(FLAG_VSYNC_HINT);

    Ball ball;
    ball.x = GetScreenWidth() / 2;
    ball.y = GetScreenHeight() / 2;
    ball.radius = 5;
    ball.speedX = 0;
    ball.speedY = 0;

    Paddle leftPaddle;
    leftPaddle.x = 50;
    leftPaddle.y = GetScreenHeight() / 2;
    leftPaddle.width = 10;
    leftPaddle.height = 100;
    leftPaddle.speed = 500;

    Paddle rightPaddle;
    rightPaddle.x = GetScreenWidth() - 50;
    rightPaddle.y = GetScreenHeight() / 2;
    rightPaddle.width = 10;
    rightPaddle.height = 100;
    rightPaddle.speed = 500;

    Player leftPlayer;
    leftPlayer.score = 0;

    Player rightPlayer;
    rightPlayer.score = 0;

    const int WINNING_SCORE = 5;
    const char *winnerText = nullptr;

    while (!WindowShouldClose())
    {
        ball.x += ball.speedX * GetFrameTime();
        ball.y += ball.speedY * GetFrameTime();

        if (ball.y > GetScreenHeight() - ball.radius)
        {
            ball.speedY = -ball.speedY;
            ball.y = GetScreenHeight() - ball.radius;
        }
        else if (ball.y < ball.radius)
        {
            ball.speedY = -ball.speedY;
            ball.y = ball.radius;
        }

        if (IsKeyDown(KEY_E) && leftPaddle.y - leftPaddle.height / 2 > 0)
        {
            leftPaddle.y -= leftPaddle.speed * GetFrameTime();
        }
        else if (IsKeyDown(KEY_D) && leftPaddle.y + leftPaddle.height / 2 < GetScreenHeight())
        {
            leftPaddle.y += leftPaddle.speed * GetFrameTime();
        }

        if (IsKeyDown(KEY_I) && rightPaddle.y - rightPaddle.height / 2 > 0)
        {
            rightPaddle.y -= rightPaddle.speed * GetFrameTime();
        }
        else if (IsKeyDown(KEY_K) && rightPaddle.y + rightPaddle.height / 2 < GetScreenHeight())
        {
            rightPaddle.y += rightPaddle.speed * GetFrameTime();
        }

        if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, leftPaddle.GetRectangle()))
        {
            if (ball.speedX < 0)
            {
                ball.speedX *= -1.1f;
                ball.speedY = (ball.y - leftPaddle.y) / (leftPaddle.height / 2) * ball.speedX;
            }
        }
        else if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, rightPaddle.GetRectangle()))
        {
            if (ball.speedX > 0)
            {
                ball.speedX *= -1.1f;
                ball.speedY = (ball.y - rightPaddle.y) / (rightPaddle.height / 2) * -ball.speedX;
            }
        }

        if (ball.x < 0)
        {
            ball.x = GetScreenWidth() / 2;
            ball.y = GetScreenHeight() / 2;
            ball.speedX = 0;
            ball.speedY = 0;
            rightPlayer.score++;
            if (rightPlayer.score >= WINNING_SCORE)
            {
                winnerText = "Right player wins!";
            }
        }
        else if (ball.x > GetScreenWidth())
        {
            ball.x = GetScreenWidth() / 2;
            ball.y = GetScreenHeight() / 2;
            ball.speedX = 0;
            ball.speedY = 0;
            leftPlayer.score++;
            if (leftPlayer.score >= WINNING_SCORE)
            {
                winnerText = "Left player wins!";
            }
        }
        if (IsKeyPressed(KEY_SPACE))
        {
            ball.speedX = rand() % 2 == 0 ? 300 : -300;
            ball.speedY = rand() % 2 == 0 ? 300 : -300;
            winnerText = nullptr;
        }

        BeginDrawing();
        ClearBackground(BLACK);
        ball.draw();
        leftPaddle.draw();
        rightPaddle.draw();
        leftPlayer.drawScore(GetScreenWidth() / 4);
        rightPlayer.drawScore(GetScreenWidth() / 4 * 3);

        if (winnerText)
        {
            int winnerTextWidth = MeasureText(winnerText, 60);
            DrawText(winnerText, GetScreenWidth() / 2 - winnerTextWidth / 2, GetScreenHeight() / 2 - 30, 60, YELLOW);
            leftPlayer.score = 0;
            rightPlayer.score = 0;
        }

        DrawFPS(10, 10);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}