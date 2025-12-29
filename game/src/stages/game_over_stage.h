/**
 * @file game_over_stage.h
 * @brief Game over screen stage implementation
 *
 * Displays the game over screen with final score, animated background,
 * and flashing prompt text. Waits for player input to restart the game
 * or quit, showing copyright information and score results.
 */

#ifndef GAME_SRC_STAGES_GAME_OVER_STAGE_H_
#define GAME_SRC_STAGES_GAME_OVER_STAGE_H_

#include "asteroid.h"
#include "game.h"
#include "geometry.h"
#include "text.h"

#define GAME_OVER_ASTEROIDS_COUNT 150

typedef struct {
  game_ptr game;
  graphics_context_ptr graphics_context;
  audio_context_ptr audio_context;

  int title_text_scale;
  text_dimensions_t title_text_dimensions;
  point_t title_text_position;

  int score_text_scale;
  char score_text[100];

  int action_text_scale;
  text_dimensions_t action_text_dimensions;
  point_t action_text_position;
  bool is_action_text_on;
  int last_action_text_ticks;

  int copyright_text_scale;
  text_dimensions_t copyright_text_dimensions;

  asteroid_t asteroids[GAME_OVER_ASTEROIDS_COUNT];
} game_over_stage_state_t;

typedef game_over_stage_state_t* game_over_stage_state_ptr;

game_over_stage_state_ptr create_game_over_stage(game_ptr game);
void destroy_game_over_stage(game_over_stage_state_ptr state);
game_stage_action_t handle_game_over_stage(game_over_stage_state_ptr state);

#endif  // GAME_SRC_STAGES_GAME_OVER_STAGE_H_
