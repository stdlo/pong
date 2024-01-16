#include "raylib.h"

typedef struct Ball {
  float x, y;
  float speedX, speedY;
  float radius;
} Ball;

typedef struct Paddle {
  float x, y;
  float speed;
  float width;
  float height;
} Paddle;

// lol this for sure is not an ideal way of doing this
int GetRandomValueExceptZero(int a, int b) {
  int x = GetRandomValue(a, b);
  if (x == 0) return GetRandomValueExceptZero(a, b);
  return x;
}
void ResetBall(Ball *ball) {
  (*ball).x = GetScreenWidth() / 2.0f;
  (*ball).y = GetScreenHeight() / 2.0f;
  (*ball).speedX = GetRandomValueExceptZero(-1, 1) * 300;
  (*ball).speedY = GetRandomValueExceptZero(-1, 1) * 300;
}
void DrawBall(Ball ball) {
  DrawCircle((int)ball.x, (int)ball.y, ball.radius, WHITE);
}

Rectangle GetRect(Paddle paddle) {
  Rectangle r;
  r.x = paddle.x - paddle.width / 2;
  r.y = paddle.y - paddle.height / 2;
  r.width = 10;
  r.height = 100;
  return r;
}
void DrawPaddle(Paddle paddle) { DrawRectangleRec(GetRect(paddle), WHITE); }

int main(void) {
  const int screenWidth = 800;
  const int screenHeight = 450;

  InitWindow(screenWidth, screenHeight, "PONG");
  /* SetTargetFPS(60); */
  SetWindowState(FLAG_VSYNC_HINT);

  int leftScore = 0;
  int rightScore = 0;
  int winner = 0;
  char* winnerText = "";

  Ball ball;
  ball.radius = 5;
  ResetBall(&ball);
  /* ball.x = GetScreenWidth() / 2.0f; */
  /* ball.y = GetScreenHeight() / 2.0f; */
  /* ball.speedX = 300; */
  /* ball.speedY = 300; */

  Paddle leftPaddle;
  leftPaddle.x = 50;
  leftPaddle.y = GetScreenHeight() / 2.0f;
  leftPaddle.width = 10;
  leftPaddle.height = 100;
  leftPaddle.speed = 500;

  Paddle rightPaddle;
  rightPaddle.x = GetScreenWidth() - 50.0f;
  rightPaddle.y = GetScreenHeight() / 2.0f;
  rightPaddle.width = 10;
  rightPaddle.height = 100;
  rightPaddle.speed = 500;

  while (!WindowShouldClose()) {
    // Update
    ball.x += ball.speedX * GetFrameTime();
    ball.y += ball.speedY * GetFrameTime();

    // Handle bouncing on top and bottom
    if (ball.y < 0) {
      ball.y = 0;
      ball.speedY *= -1;
    }
    if (ball.y > GetScreenHeight()) {
      ball.y = GetScreenHeight();
      ball.speedY *= -1;
    }

    // Read keypresses
    if (IsKeyDown(KEY_W))
      leftPaddle.y -= leftPaddle.speed * GetFrameTime();
    if (IsKeyDown(KEY_S))
      leftPaddle.y += leftPaddle.speed * GetFrameTime();

    if (IsKeyDown(KEY_K))
      rightPaddle.y -= rightPaddle.speed * GetFrameTime();
    if (IsKeyDown(KEY_J))
      rightPaddle.y += rightPaddle.speed * GetFrameTime();

    // ball collision
    if (CheckCollisionCircleRec((Vector2){ball.x, ball.y}, ball.radius, GetRect(leftPaddle))) {
      if (ball.speedX < 0) {
        ball.speedX *= -1.1f;
        ball.speedY = (ball.y - leftPaddle.y) / (leftPaddle.height / 2) * ball.speedX;
      }
    }
    if (CheckCollisionCircleRec((Vector2){ball.x, ball.y}, ball.radius, GetRect(rightPaddle))) {
      if (ball.speedX > 0) {
        ball.speedX *= -1.1f;
        ball.speedY = (ball.y - rightPaddle.y) / (rightPaddle.height / 2) * -ball.speedX;
      }
    }

    /* float leftPaddleTop = leftPaddle.y - leftPaddle.height / 2; */
    /* float leftPaddleBottom = leftPaddle.y + leftPaddle.height / 2; */
    // paddle collision
    if ((leftPaddle.y + leftPaddle.height / 2) >= GetScreenHeight()) leftPaddle.y = GetScreenHeight() - leftPaddle.height / 2;
    else if ((leftPaddle.y - leftPaddle.height / 2) <= 0) leftPaddle.y = leftPaddle.height / 2;

    if ((rightPaddle.y + rightPaddle.height / 2) >= GetScreenHeight()) rightPaddle.y = GetScreenHeight() - rightPaddle.height / 2;
    else if ((rightPaddle.y - rightPaddle.height / 2) <= 0) rightPaddle.y = rightPaddle.height / 2;
    /* if (((leftPaddle.y + leftPaddle.height) >= GetScreenHeight()) || (leftPaddle.y <= 0)) leftPaddle.speed = 0; */
    /* if (((rightPaddle.y + rightPaddle.height) >= GetScreenHeight()) || (rightPaddle.y <= 0)) rightPaddle.speed = 0; */

    if (ball.x < 0) {
      leftScore += 1;
      ResetBall(&ball);
    }
    if (ball.x > GetScreenWidth()) {
      rightScore += 1;
      ResetBall(&ball);
    }

    if (rightScore > 9) {
      winner = 1;
      winnerText = "RIGHT PLAYER WINS!";
      ResetBall(&ball);
      ball.speedX = 0;
      ball.speedY = 0;
    }
    if (leftScore > 9) {
      winner = 1;
      winnerText = "LEFT PLAYER WINS!";
      ResetBall(&ball);
      ball.speedX = 0;
      ball.speedY = 0;
    }
    if (winner == 1 && IsKeyPressed(KEY_SPACE)) {
      ball.speedX = 300;
      ball.speedY = 300;
      leftPaddle.x = 50;
      leftPaddle.y = GetScreenHeight() / 2.0f;
      rightPaddle.x = GetScreenWidth() - 50.0f;
      rightPaddle.y = GetScreenHeight() / 2.0f;
      leftScore = 0;
      rightScore = 0;
      winner = 0;
    }

    // Draw
    BeginDrawing();
    {
      ClearBackground(BLACK);

      if (winner == 1) {
        DrawText(winnerText, (GetScreenWidth() / 2) - (MeasureText(winnerText, 50) / 2), 150, 50, WHITE);
      } else {
        DrawBall(ball);
        DrawPaddle(leftPaddle);
        DrawPaddle(rightPaddle);
      }

      DrawText(TextFormat("%d", leftScore), 50, 50, 30, WHITE);
      DrawText(TextFormat("%d", rightScore), GetScreenWidth() - 50, 50, 30,
               WHITE);

      DrawFPS(10, 10);
    }
    EndDrawing();
  }

  // De-Initialization
  CloseWindow();

  return 0;
}
