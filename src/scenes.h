#ifndef SCENES_H
#define SCENES_H

struct scene_manager;
typedef void scene_fn(struct scene_manager *);

struct ball {
  float x, y;
  float speed_x, speed_y;
  float radius;
};

typedef struct paddle {
  float x, y;
  float speed;
  float width;
  float height;
} paddle;

struct scene_manager {
  scene_fn *next;
  int left_score;
  int right_score;
  // paddle left_paddle;
  // paddle right_paddle;
  // struct ball ball;
};

scene_fn title_scene, new_game_scene, play_scene, pause_scene, end_scene;

#endif
