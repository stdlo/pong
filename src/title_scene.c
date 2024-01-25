#include "raylib.h"
#include "scenes.h"

void title_scene(struct scene_manager *scene) {
  if (GetKeyPressed() != 0) scene->next = new_game_scene;

  char *title = "PONG";
  char *message = "press any key to play";

  BeginDrawing();
  {
    ClearBackground(BLACK);

    DrawText(title, (GetScreenWidth() / 2) - (MeasureText(title, 50) / 2), 150, 50, WHITE);
    DrawText(message, (GetScreenWidth() / 2) - (MeasureText(message, 25) / 2), 300, 25, WHITE);

    DrawFPS(10, 10);
  }
  EndDrawing();
}
