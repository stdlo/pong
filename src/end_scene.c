#include "raylib.h"
#include "scenes.h"

void end_scene(struct scene_manager *scene) {
  if (GetKeyPressed() != 0) scene->next = new_game_scene;

  char *winner_text = scene->right_score > scene->left_score ? "PLAYER 2 WINS!"
                                                             : "PLAYER 1 WINS!";
  char *message = "press any key to play again";

  BeginDrawing();
  {
    ClearBackground(BLACK);

    DrawText(winner_text, (GetScreenWidth() / 2) - (MeasureText(winner_text, 50) / 2), 150, 50, WHITE);
    DrawText(message, (GetScreenWidth() / 2) - (MeasureText(message, 25) / 2), 300, 25, WHITE);

    DrawFPS(10, 10);
  }
  EndDrawing();
}
