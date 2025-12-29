#include "game_over_stage.h"

#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

#include "animate.h"
#include "asteroid.h"
#include "audio.h"
#include "background_effects.h"
#include "clock.h"
#include "events.h"
#include "frame.h"
#include "frame_limiter.h"
#include "game.h"
#include "game_audio.h"
#include "geometry.h"
#include "graphics.h"
#include "keyboard.h"
#include "physics.h"
#include "sprites.h"
#include "stage.h"
#include "string.h"
#include "text.h"

#define TITLE_TEXT "GAME OVER"
#define ACTION_TEXT "PRESS RETURN TO RESTART GAME"
#define ACTION_TEXT_FLASHING_TICKS 750

game_over_stage_state_ptr create_game_over_stage(game_ptr game) {
  game_over_stage_state_ptr state = malloc(sizeof(game_over_stage_state_t));
  state->game = game;
  state->graphics_context = &game->graphics_context;
  state->audio_context = &game->audio_context;

  state->score_text_scale = (state->graphics_context->screen_height * 5) / 450;
  state->title_text_scale = (state->graphics_context->screen_height * 25) / 900;
  state->title_text_dimensions =
      calculate_text_dimensions(TITLE_TEXT, state->title_text_scale);
  state->title_text_position = point(
      state->graphics_context->screen_center.x -
          state->title_text_dimensions.width / 2,
      state->graphics_context->screen_center.y -
          state->graphics_context->screen_height / 6);

  state->action_text_scale =
      (state->graphics_context->screen_height * 10) / 900;
  state->action_text_dimensions =
      calculate_text_dimensions(ACTION_TEXT, state->action_text_scale);
  state->action_text_position = point(
      state->graphics_context->screen_center.x -
          state->action_text_dimensions.width / 2,
      state->graphics_context->screen_center.y +
          state->graphics_context->screen_height / 6);

  state->copyright_text_scale =
      (state->graphics_context->screen_height * 5) / 900;
  state->copyright_text_dimensions =
      calculate_text_dimensions(COPYRIGHT_TEXT, state->copyright_text_scale);

  state->is_action_text_on = true;
  state->last_action_text_ticks = 0;
  memset(state->score_text, 0, sizeof(state->score_text));

  return state;
}

void destroy_game_over_stage(game_over_stage_state_ptr state) {
  if (state != NULL) {
    free(state);
  }
}

static void play_game_over_if_sound_on(game_over_stage_state_ptr state) {
  if (state->game->settings.volume > 0) {
    play_game_over(state->audio_context);
  }
}


static ALWAYS_INLINE void show_score(game_over_stage_state_ptr state) {
  snprintf(state->score_text, sizeof state->score_text, "SCORE %d",
           state->game->score);
  text_dimensions_t score_text_dimensions =
      calculate_text_dimensions(state->score_text, state->score_text_scale);
  point_t score_text_position =
      point(state->graphics_context->screen_center.x -
                score_text_dimensions.width / 2,
            state->graphics_context->screen_center.y);
  write_text(state->graphics_context, state->score_text,
             score_text_position, state->score_text_scale, COLOR_YELLOW);
}

static ALWAYS_INLINE void show_copyright(game_over_stage_state_ptr state) {
  write_text(state->graphics_context, COPYRIGHT_TEXT,
             point(state->graphics_context->screen_center.x -
                       state->copyright_text_dimensions.width / 2,
                   state->graphics_context->screen_height -
                       state->copyright_text_dimensions.height - 5),
             state->copyright_text_scale, COLOR_DARK_YELLOW);
}

static ALWAYS_INLINE void animate_action_text(
    game_over_stage_state_ptr state) {
  if (elapsed_from(state->last_action_text_ticks) >
      ACTION_TEXT_FLASHING_TICKS) {
    state->is_action_text_on = !state->is_action_text_on;
    state->last_action_text_ticks = get_clock_ticks_ms();
  }
  if (state->is_action_text_on) {
    write_text(state->graphics_context, ACTION_TEXT,
               state->action_text_position, state->action_text_scale,
               COLOR_WHITE);
  }
}

static ALWAYS_INLINE void show_title_text(game_over_stage_state_ptr state) {
  write_text(state->graphics_context, TITLE_TEXT,
             state->title_text_position, state->title_text_scale,
             COLOR_YELLOW);
}

game_stage_action_t handle_game_over_stage(game_over_stage_state_ptr state) {
  play_game_over_if_sound_on(state);

  frame_limiter_t frame_limiter =
      create_frame_limiter(state->game->settings.fps);

  init_background_asteroids(state->asteroids, GAME_OVER_ASTEROIDS_COUNT,
                             state->graphics_context);

  while (true) {
    double delta_time = frame_limiter_wait(&frame_limiter);

    clear_frame(state->graphics_context);

    animate_background_asteroids(state->asteroids, GAME_OVER_ASTEROIDS_COUNT,
                                  state->graphics_context, delta_time);

    animate_action_text(state);

    show_title_text(state);

    show_score(state);

    show_copyright(state);

    render_frame(state->graphics_context);

    event_t event;

    while ((event = poll_event())) {
      switch (event) {
        case NO_EVENT:
        case KEY_PRESSED_EVENT:
        case OTHER_EVENT:
          break;
        case QUIT_EVENT: {
          return QUIT;
        }
      }
    }

    if (is_return_key_pressed(&state->game->keyboard_state)) {
      return PROGRESS;
    }

    if (is_esc_key_pressed(&state->game->keyboard_state)) {
      return QUIT;
    }
  }
}

/* ---- ==== Stage Interface Implementation ==== ---- */

static void game_over_init(stage_ptr stage, game_ptr game) {
  game_over_stage_state_ptr state = create_game_over_stage(game);
  stage->state = state;
}

static game_stage_action_t game_over_update(stage_ptr stage) {
  game_over_stage_state_ptr state = (game_over_stage_state_ptr)stage->state;
  return handle_game_over_stage(state);
}

static void game_over_cleanup(stage_ptr stage) {
  game_over_stage_state_ptr state = (game_over_stage_state_ptr)stage->state;
  destroy_game_over_stage(state);
  stage->state = NULL;
}

stage_ptr create_game_over_stage_instance(void) {
  stage_ptr stage = malloc(sizeof(stage_t));
  stage->state = NULL;
  stage->init = game_over_init;
  stage->update = game_over_update;
  stage->cleanup = game_over_cleanup;
  stage->name = "GAME_OVER";
  return stage;
}
