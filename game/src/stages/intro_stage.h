#ifndef GAME_SRC_STAGES_INTRO_STAGE_H_
#define GAME_SRC_STAGES_INTRO_STAGE_H_

#include "asteroid.h"
#include "game.h"
#include "geometry.h"
#include "text.h"

#define INTRO_ASTEROIDS_COUNT 150

typedef struct {
  game_ptr game;
  graphics_context_ptr graphics_context;

  int title_text_scale;
  text_dimensions_t title_text_dimensions;
  point_t title_text_position;

  int instructions_text_scale;
  text_dimensions_t instructions_text_dimensions;
  point_t instructions_text_0_position;
  point_t instructions_text_1_position;
  point_t instructions_text_2_position;
  point_t instructions_text_3_position;
  point_t instructions_text_4_position;

  int action_text_scale;
  text_dimensions_t action_text_dimensions;
  point_t action_text_position;

  int copyright_text_scale;
  text_dimensions_t copyright_text_dimensions;

  asteroid_t asteroids[INTRO_ASTEROIDS_COUNT];
} intro_stage_state_t;

typedef intro_stage_state_t* intro_stage_state_ptr;

intro_stage_state_ptr create_intro_stage(game_ptr game);
void destroy_intro_stage(intro_stage_state_ptr state);
game_stage_action_t handle_intro_stage(intro_stage_state_ptr state);

#endif  // GAME_SRC_STAGES_INTRO_STAGE_H_
