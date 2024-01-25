#include "raylib.h"
#include "scenes.h"

#define MAX_SCORE 10

// global game objects
static paddle left_paddle, right_paddle;
static struct ball ball;

//------------------------------------------------------------------------------
// ball functions
//------------------------------------------------------------------------------
int get_direction() {
  int rand_value = GetRandomValue(0, 1);
  int result = rand_value > 0 ? 1 : -1;
  return result;
}
void reset_ball() {
  ball.x = GetScreenWidth() / 2.0f;
  ball.y = GetScreenHeight() / 2.0f;
  ball.speed_x = get_direction() * 300;
  ball.speed_y = get_direction() * 300;
}
void draw_ball(Color color) { DrawCircle((int)ball.x, (int)ball.y, ball.radius, color); }

//------------------------------------------------------------------------------
// paddle functions
//------------------------------------------------------------------------------
Rectangle get_rect(paddle *paddle) {
  Rectangle r;
  r.x = paddle->x - paddle->width / 2;
  r.y = paddle->y - paddle->height / 2;
  r.width = 10;
  r.height = 100;
  return r;
}
void draw_paddle(paddle *paddle, Color color) { DrawRectangleRec(get_rect(paddle), color); }

void new_game_scene(struct scene_manager *scene) {
  scene->next = play_scene;

  left_paddle.x = 50;
  left_paddle.y = GetScreenHeight() / 2.0f;
  left_paddle.width = 10;
  left_paddle.height = 100;
  left_paddle.speed = 500;

  right_paddle.x = GetScreenWidth() - 50.0f;
  right_paddle.y = GetScreenHeight() / 2.0f;
  right_paddle.width = 10;
  right_paddle.height = 100;
  right_paddle.speed = 500;

  ball.radius = 5;
  ball.speed_x = 300;
  ball.speed_y = 300;
  reset_ball();

  scene->left_score = 0;
  scene->right_score = 0;
}

void play_scene(struct scene_manager *scene) {
  //------------------------------------------------------------------------------
  // UPDATE
  //------------------------------------------------------------------------------
  // read keypresses
  if (IsKeyDown(KEY_W) || IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_UP))
    left_paddle.y -= left_paddle.speed * GetFrameTime();
  if (IsKeyDown(KEY_S) || IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN))
    left_paddle.y += left_paddle.speed * GetFrameTime();


  if (IsKeyDown(KEY_O) || IsGamepadButtonDown(1, GAMEPAD_BUTTON_LEFT_FACE_UP))
    right_paddle.y -= right_paddle.speed * GetFrameTime();
  if (IsKeyDown(KEY_L) || IsGamepadButtonDown(1, GAMEPAD_BUTTON_LEFT_FACE_DOWN))
    right_paddle.y += right_paddle.speed * GetFrameTime();

  // handle pause
  if (IsKeyDown(KEY_ESCAPE)) {
    scene->next = pause_scene;
    return;
  }

  // move the ball
  ball.x += ball.speed_x * GetFrameTime();
  ball.y += ball.speed_y * GetFrameTime();

  // collision - ball bouncing on top and bottom
  if (ball.y < 0) {
    ball.y = 0;
    ball.speed_y *= -1;
  }
  if (ball.y > GetScreenHeight()) {
    ball.y = GetScreenHeight();
    ball.speed_y *= -1;
  }

  // collision - ball on paddle
  if (CheckCollisionCircleRec((Vector2){ball.x, ball.y}, ball.radius,
                              get_rect(&left_paddle))) {
    if (ball.speed_x < 0) {
      ball.speed_x *= -1.1f;
      ball.speed_y =
          (ball.y - left_paddle.y) / (left_paddle.height / 2) * ball.speed_x;
    }
  }
  if (CheckCollisionCircleRec((Vector2){ball.x, ball.y}, ball.radius,
                              get_rect(&right_paddle))) {
    if (ball.speed_x > 0) {
      ball.speed_x *= -1.1f;
      ball.speed_y =
          (ball.y - right_paddle.y) / (right_paddle.height / 2) * -ball.speed_x;
    }
  }

  // collision - paddle against top and bottom
  if ((left_paddle.y + left_paddle.height / 2) >= GetScreenHeight())
    left_paddle.y = GetScreenHeight() - left_paddle.height / 2;
  else if ((left_paddle.y - left_paddle.height / 2) <= 0)
    left_paddle.y = left_paddle.height / 2;
  if ((right_paddle.y + right_paddle.height / 2) >= GetScreenHeight())
    right_paddle.y = GetScreenHeight() - right_paddle.height / 2;
  else if ((right_paddle.y - right_paddle.height / 2) <= 0)
    right_paddle.y = right_paddle.height / 2;

  // score updates
  int score_font_size = 40;
  char *left_score_text = TextFormat("%d", scene->left_score);
  int left_score_x = 100;
  char *right_score_text = TextFormat("%d", scene->right_score);
  int right_score_x = GetScreenWidth() - (100 + MeasureText(right_score_text, score_font_size));

  //------------------------------------------------------------------------------
  // LOGIC
  //------------------------------------------------------------------------------

  // keep score and reset ball on score
  if (ball.x < 0) {
    scene->right_score++;
    reset_ball();
  } else if (ball.x > GetScreenWidth()) {
    scene->left_score++;
    reset_ball();
  }

  // set next scene on win
  if (scene->left_score >= MAX_SCORE || scene->right_score >= MAX_SCORE) scene->next = end_scene;

  //------------------------------------------------------------------------------
  // DRAW
  //------------------------------------------------------------------------------
  BeginDrawing();
  {
    ClearBackground(BLACK);

    draw_ball(WHITE);
    draw_paddle(&left_paddle, WHITE);
    draw_paddle(&right_paddle, WHITE);

    DrawText(left_score_text, left_score_x, 50, 40, WHITE);
    DrawText(right_score_text, right_score_x, 50, 40, WHITE);

    DrawFPS(10, 10);
  }
  EndDrawing();
}
