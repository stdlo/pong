#include "raylib.h"
#include "scenes.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450

int main() {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "PONG");
  SetTargetFPS(60);
  SetExitKey(0);

  struct scene_manager scene = { title_scene };
  while (!WindowShouldClose()) {
    scene.next(&scene);
  }

  CloseWindow();
  return 0;
}
