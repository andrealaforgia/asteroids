#include "intro_stage.h"

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
#include "geometry.h"
#include "graphics.h"
#include "keyboard.h"
#include "physics.h"
#include "sprites.h"
#include "saucer.h"
#include "stage.h"
#include "text.h"

#define TITLE_TEXT "ASTEROIDS"
#define ACTION_TEXT "PRESS SPACE TO START"
#define ACTION_TEXT_FLASHING_TICKS 750

#define INSTRUCTIONS_TEXT_0 "        PRESS UP OR K TO THRUST SHIP        "
#define INSTRUCTIONS_TEXT_1 "PRESS LEFT OR H OR RIGHT OR L TO ROTATE SHIP"
#define INSTRUCTIONS_TEXT_2 "            PRESS SPACE TO SHOOT            "
#define INSTRUCTIONS_TEXT_3 "     PRESS S TO ENABLE OR DISABLE SOUND     "
#define INSTRUCTIONS_TEXT_4 "              PRESS ESC TO EXIT             "

intro_stage_state_ptr create_intro_stage(game_ptr game) {
  intro_stage_state_ptr state = malloc(sizeof(intro_stage_state_t));
  state->game = game;
  state->graphics_context = &game->graphics_context;

  state->title_text_scale =
      (state->graphics_context->screen_height * 25) / 900;
  state->title_text_dimensions =
      calculate_text_dimensions(TITLE_TEXT, state->title_text_scale);
  state->title_text_position = point(
      state->graphics_context->screen_center.x -
          state->title_text_dimensions.width / 2,
      state->graphics_context->screen_center.y -
          state->graphics_context->screen_height / 3);

  state->action_text_scale =
      (state->graphics_context->screen_height * 10) / 900;
  state->action_text_dimensions =
      calculate_text_dimensions(ACTION_TEXT, state->action_text_scale);
  state->action_text_position = point(
      state->graphics_context->screen_center.x -
          state->action_text_dimensions.width / 2,
      state->graphics_context->screen_center.y +
          state->graphics_context->screen_height / 6);

  state->instructions_text_scale =
      (state->graphics_context->screen_height * 10) / 900;
  state->instructions_text_dimensions = calculate_text_dimensions(
      INSTRUCTIONS_TEXT_0, state->instructions_text_scale);
  state->instructions_text_0_position = point(
      state->graphics_context->screen_center.x -
          state->instructions_text_dimensions.width / 2,
      state->graphics_context->screen_center.y -
          state->graphics_context->screen_height / 4);

  state->instructions_text_1_position = point(
      state->graphics_context->screen_center.x -
          state->instructions_text_dimensions.width / 2,
      state->graphics_context->screen_center.y -
          state->graphics_context->screen_height / 4 +
          state->instructions_text_dimensions.height +
          state->instructions_text_dimensions.height / 2);

  state->instructions_text_2_position = point(
      state->graphics_context->screen_center.x -
          state->instructions_text_dimensions.width / 2,
      state->graphics_context->screen_center.y -
          state->graphics_context->screen_height / 4 +
          state->instructions_text_dimensions.height * 2 +
          2 * state->instructions_text_dimensions.height / 2);

  state->instructions_text_3_position = point(
      state->graphics_context->screen_center.x -
          state->instructions_text_dimensions.width / 2,
      state->graphics_context->screen_center.y -
          state->graphics_context->screen_height / 4 +
          state->instructions_text_dimensions.height * 3 +
          3 * state->instructions_text_dimensions.height / 2);

  state->instructions_text_4_position = point(
      state->graphics_context->screen_center.x -
          state->instructions_text_dimensions.width / 2,
      state->graphics_context->screen_center.y -
          state->graphics_context->screen_height / 4 +
          state->instructions_text_dimensions.height * 4 +
          4 * state->instructions_text_dimensions.height / 2);

  state->copyright_text_scale =
      (state->graphics_context->screen_height * 5) / 900;
  state->copyright_text_dimensions =
      calculate_text_dimensions(COPYRIGHT_TEXT, state->copyright_text_scale);

  return state;
}

void destroy_intro_stage(intro_stage_state_ptr state) {
  if (state != NULL) {
    free(state);
  }
}

/* ---- ==== Stage Interface Implementation ==== ---- */

static void intro_init(stage_ptr stage, game_ptr game) {
  intro_stage_state_ptr state = create_intro_stage(game);
  stage->state = state;
}

static game_stage_action_t intro_update(stage_ptr stage) {
  intro_stage_state_ptr state = (intro_stage_state_ptr)stage->state;
  return handle_intro_stage(state);
}

static void intro_cleanup(stage_ptr stage) {
  intro_stage_state_ptr state = (intro_stage_state_ptr)stage->state;
  destroy_intro_stage(state);
  stage->state = NULL;
}

stage_ptr create_intro_stage_instance(void) {
  stage_ptr stage = malloc(sizeof(stage_t));
  stage->state = NULL;
  stage->init = intro_init;
  stage->update = intro_update;
  stage->cleanup = intro_cleanup;
  stage->name = "INTRO";
  return stage;
}

game_stage_action_t handle_intro_stage(intro_stage_state_ptr state) {
  int last_action_text_ticks = get_clock_ticks_ms();

  bool is_action_text_on = true;

  init_background_asteroids(state->asteroids, INTRO_ASTEROIDS_COUNT,
                             state->graphics_context);

  frame_limiter_t frame_limiter =
      create_frame_limiter(state->game->settings.fps);

  while (true) {
    double delta_time = frame_limiter_wait(&frame_limiter);

    clear_frame(state->graphics_context);

    animate_background_asteroids(state->asteroids, INTRO_ASTEROIDS_COUNT,
                                  state->graphics_context, delta_time);

    write_text(state->graphics_context, INSTRUCTIONS_TEXT_0,
               state->instructions_text_0_position,
               state->instructions_text_scale, COLOR_GRAY);

    write_text(state->graphics_context, INSTRUCTIONS_TEXT_1,
               state->instructions_text_1_position,
               state->instructions_text_scale, COLOR_GRAY);

    write_text(state->graphics_context, INSTRUCTIONS_TEXT_2,
               state->instructions_text_2_position,
               state->instructions_text_scale, COLOR_GRAY);

    write_text(state->graphics_context, INSTRUCTIONS_TEXT_3,
               state->instructions_text_3_position,
               state->instructions_text_scale, COLOR_GRAY);

    write_text(state->graphics_context, INSTRUCTIONS_TEXT_4,
               state->instructions_text_4_position,
               state->instructions_text_scale, COLOR_GRAY);

    if (elapsed_from(last_action_text_ticks) >
        ACTION_TEXT_FLASHING_TICKS) {
      is_action_text_on = !is_action_text_on;
      last_action_text_ticks = get_clock_ticks_ms();
    }

    write_text(state->graphics_context, TITLE_TEXT,
               state->title_text_position, state->title_text_scale,
               COLOR_YELLOW);

    if (is_action_text_on) {
      write_text(state->graphics_context, ACTION_TEXT,
                 state->action_text_position, state->action_text_scale,
                 COLOR_WHITE);
    }

    write_text(state->graphics_context, COPYRIGHT_TEXT,
               point(state->graphics_context->screen_center.x -
                         state->copyright_text_dimensions.width / 2,
                     state->graphics_context->screen_height -
                         state->copyright_text_dimensions.height - 5),
               state->copyright_text_scale, COLOR_DARK_YELLOW);

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

    if (is_space_key_pressed(&state->game->keyboard_state)) {
      return PROGRESS;
    }

    if (is_s_key_pressed(&state->game->keyboard_state)) {
      // Toggle sound: if volume > 0, mute; otherwise set to default
      if (state->game->settings.volume > 0) {
        state->game->settings.volume = 0;
        set_audio_volume(0);  // Mute SDL mixer
      } else {
        state->game->settings.volume = 50;  // Default volume
        set_audio_volume(64);  // Set SDL mixer to 50% (64/128)
      }
    }

    if (is_esc_key_pressed(&state->game->keyboard_state)) {
      return QUIT;
    }
  }
}
