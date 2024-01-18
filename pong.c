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

int GetDirection() {
  int rand_value = GetRandomValue(0, 1);
  int result = rand_value > 0 ? 1 : -1;
  /* TraceLog(LOG_INFO, TextFormat("rand_value=%d result=%d", rand_value, result)); */
  return result;
}

void ResetBall(Ball *ball) {
  (*ball).x = GetScreenWidth() / 2.0f;
  (*ball).y = GetScreenHeight() / 2.0f;
  /* (*ball).speedX = GetRandomValueExceptZero(-1, 1) * 300; */
  /* (*ball).speedY = GetRandomValueExceptZero(-1, 1) * 300; */
  (*ball).speedX = GetDirection() * 300;
  (*ball).speedY = GetDirection() * 300;
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

void HandleInput(Paddle *leftPaddle, Paddle *rightPaddle) {
  // Read keypresses
  if (IsKeyDown(KEY_W) || IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_UP))
    (*leftPaddle).y -= (*leftPaddle).speed * GetFrameTime();
  if (IsKeyDown(KEY_S) || IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN))
    (*leftPaddle).y += (*leftPaddle).speed * GetFrameTime();

  if (IsKeyDown(KEY_O) || IsGamepadButtonDown(1, GAMEPAD_BUTTON_LEFT_FACE_UP))
    (*rightPaddle).y -= (*rightPaddle).speed * GetFrameTime();
  if (IsKeyDown(KEY_L) || IsGamepadButtonDown(1, GAMEPAD_BUTTON_LEFT_FACE_DOWN))
    (*rightPaddle).y += (*rightPaddle).speed * GetFrameTime();
}

void HandleCollision(Paddle *leftPaddle, Paddle *rightPaddle, Ball *ball) {
  // Handle bouncing on top and bottom
  if ((*ball).y < 0) {
    (*ball).y = 0;
    (*ball).speedY *= -1;
  }
  if ((*ball).y > GetScreenHeight()) {
    (*ball).y = GetScreenHeight();
    (*ball).speedY *= -1;
  }

  // ball collision
  if (CheckCollisionCircleRec((Vector2){(*ball).x, (*ball).y}, (*ball).radius,
                              GetRect((*leftPaddle)))) {
    if ((*ball).speedX < 0) {
      (*ball).speedX *= -1.1f;
      (*ball).speedY = ((*ball).y - (*leftPaddle).y) /
                       ((*leftPaddle).height / 2) * (*ball).speedX;
    }
  }
  if (CheckCollisionCircleRec((Vector2){(*ball).x, (*ball).y}, (*ball).radius,
                              GetRect((*rightPaddle)))) {
    if ((*ball).speedX > 0) {
      (*ball).speedX *= -1.1f;
      (*ball).speedY = ((*ball).y - (*rightPaddle).y) /
                       ((*rightPaddle).height / 2) * -(*ball).speedX;
    }
  }

  // paddle collision
  if (((*leftPaddle).y + (*leftPaddle).height / 2) >= GetScreenHeight())
    (*leftPaddle).y = GetScreenHeight() - (*leftPaddle).height / 2;
  else if (((*leftPaddle).y - (*leftPaddle).height / 2) <= 0)
    (*leftPaddle).y = (*leftPaddle).height / 2;
  if (((*rightPaddle).y + (*rightPaddle).height / 2) >= GetScreenHeight())
    (*rightPaddle).y = GetScreenHeight() - (*rightPaddle).height / 2;
  else if (((*rightPaddle).y - (*rightPaddle).height / 2) <= 0)
    (*rightPaddle).y = (*rightPaddle).height / 2;
}

int main(void) {
  const int screenWidth = 800;
  const int screenHeight = 450;
  const int scoreToWin = 1;

  /* SetConfigFlags(FLAG_VSYNC_HINT); */
  /* SetWindowState(FLAG_VSYNC_HINT); */
  InitWindow(screenWidth, screenHeight, "PONG");
  SetTargetFPS(60);

  int leftScore = 0;
  int rightScore = 0;
  int winner = 0;
  char *winnerText = "";

  Ball ball;
  ball.radius = 5;
  ResetBall(&ball);

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
    // this blocks the loop so we dont keep rendering when theres a winner
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
    } else if (winner != 1) {
      // MOVE THE BALL
      ball.x += ball.speedX * GetFrameTime();
      ball.y += ball.speedY * GetFrameTime();

      // HANDLE INPUT AND COLLISION
      HandleInput(&leftPaddle, &rightPaddle);
      HandleCollision(&leftPaddle, &rightPaddle, &ball);

      // GAME LOGIC
      if (ball.x < 0) {
        rightScore += 1;
        ResetBall(&ball);
      }
      if (ball.x > GetScreenWidth()) {
        leftScore += 1;
        ResetBall(&ball);
      }

      if (rightScore >= scoreToWin) {
        winner = 1;
        winnerText = "PLAYER 2 WINS!";
        ResetBall(&ball);
        ball.speedX = 0;
        ball.speedY = 0;
      }
      if (leftScore >= scoreToWin) {
        winner = 1;
        winnerText = "PLAYER 1 WINS!";
        ResetBall(&ball);
        ball.speedX = 0;
        ball.speedY = 0;
      }
    }

    int scoreFontSize = 40;
    char* leftScoreText = TextFormat("%d", leftScore);
    int leftScoreX = 100;
    char* rightScoreText = TextFormat("%d", rightScore);
    int rightScoreX = GetScreenWidth() - (100 + MeasureText(rightScoreText, scoreFontSize));
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

      DrawText(leftScoreText, leftScoreX, 50, 40, WHITE);
      DrawText(rightScoreText, rightScoreX, 50, 40, WHITE);

      DrawFPS(10, 10);
    }
    EndDrawing();
  }
  CloseWindow();
  return 0;
}
