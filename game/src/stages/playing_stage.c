#include "playing_stage.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "asteroid_manager.h"
#include "bullet_manager.h"
#include "collision_system.h"
#include "events.h"
#include "frame.h"
#include "frame_limiter.h"
#include "game.h"
#include "game_audio.h"
#include "game_constants.h"
#include "game_hud.h"
#include "graphics.h"
#include "keyboard.h"
#include "saucer_manager.h"
#include "score.h"
#include "ship_controller.h"
#include "sharpnel.h"
#include "ship.h"
#include "stage.h"

/* ---- ==== ---- ==== helper functions ==== ---- ==== ---- */

static ALWAYS_INLINE bool any_ship_lives_left(const playing_stage_state_ptr state) {
  return state->game->lives > 0;
}

static ALWAYS_INLINE void consume_one_ship_life(playing_stage_state_ptr state) {
  --state->game->lives;
}

/* ---- ==== ---- ==== init ==== ---- ==== ---- */

static ALWAYS_INLINE void create_first_ship(playing_stage_state_ptr state) {
  state->ship = create_ship(state->graphics_context->screen_center, 1);
}

static ALWAYS_INLINE void reset_objects(playing_stage_state_ptr state) {
  reset_asteroids(&state->asteroid_manager);
  reset_sharpnels(state->sharpnel_system);
  reset_bullets(&state->bullet_manager);
  reset_saucer(&state->saucer_manager);
  reset_game_hud(&state->game_hud);
}

playing_stage_state_ptr create_playing_stage(game_ptr game) {
  playing_stage_state_ptr state = malloc(sizeof(playing_stage_state_t));
  state->game = game;
  state->graphics_context = &game->graphics_context;
  state->audio_context = &game->audio_context;

  // Create event system
  state->event_system = create_event_system();

  // Subscribe audio and scoring events
  subscribe_audio_events(&state->event_system, state->audio_context);
  subscribe_score_events(&state->event_system, game);

  state->sharpnel_system =
      create_sharpnel_system(state->graphics_context, MAX_SHARPNEL_COUNT);
  init_asteroid_manager(&state->asteroid_manager, game,
                        state->graphics_context, state->audio_context,
                        state->sharpnel_system, &state->event_system);
  init_bullet_manager(&state->bullet_manager, game, state->graphics_context,
                      state->audio_context);
  init_saucer_manager(&state->saucer_manager, game, state->graphics_context,
                      state->audio_context, &state->bullet_manager,
                      state->sharpnel_system, &state->event_system);
  init_game_hud(&state->game_hud, game, state->graphics_context);

  state->ship_controller = create_ship_controller(
      &state->ship, state->graphics_context, state->audio_context,
      &state->bullet_manager, state->sharpnel_system, &state->event_system,
      game->settings.volume);

  return state;
}

void destroy_playing_stage(playing_stage_state_ptr state) {
  if (state != NULL) {
    destroy_sharpnel_system(state->sharpnel_system);
    free(state);
  }
}

/* ---- ==== ---- ==== main game loop ==== ---- ==== ---- */

game_stage_action_t handle_playing_stage(playing_stage_state_ptr state) {
  frame_limiter_t frame_limiter = create_frame_limiter(state->game->settings.fps);

  create_first_ship(state);
  reset_objects(state);

  while (true) {
    double delta_time = frame_limiter_wait(&frame_limiter);

    clear_frame(state->graphics_context);

    // Create asteroids if none are left
    recreate_asteroids_if_none_are_left(&state->asteroid_manager,
                                        state->ship.position);

    // Update all game objects
    update_asteroids(&state->asteroid_manager, delta_time);
    ship_controller_update(&state->ship_controller, delta_time);
    update_ship_bullets(&state->bullet_manager, delta_time);

    if (is_saucer_flying(&state->saucer_manager)) {
      update_saucer(&state->saucer_manager, delta_time, state->ship.position);
    } else {
      create_saucer_if_required(&state->saucer_manager);
    }

    update_saucer_bullets(&state->bullet_manager, delta_time);
    animate_sharpnels(state->sharpnel_system, delta_time);

    // Check all collisions
    if (check_asteroid_ship_collisions(&state->asteroid_manager,
                                       &state->ship)) {
      ship_controller_handle_destruction(&state->ship_controller);
    }

    if (check_saucer_bullet_ship_collisions(&state->bullet_manager,
                                            &state->ship)) {
      ship_controller_handle_destruction(&state->ship_controller);
    }

    collision_result_t ship_saucer_collision =
        check_ship_saucer_collision(&state->ship, &state->saucer_manager);
    if (ship_saucer_collision.ship_destroyed) {
      ship_controller_handle_destruction(&state->ship_controller);
    }
    if (ship_saucer_collision.saucer_destroyed) {
      destroy_saucer(&state->saucer_manager);
    }

    check_ship_bullet_asteroid_collisions(&state->bullet_manager,
                                          &state->asteroid_manager);

    if (check_ship_bullet_saucer_collisions(&state->bullet_manager,
                                            &state->saucer_manager)) {
      destroy_saucer(&state->saucer_manager);
    }

    // Handle ship destruction
    if (ship_controller_is_destroyed(&state->ship_controller)) {
      if (any_ship_lives_left(state)) {
        consume_one_ship_life(state);
        ship_controller_respawn(&state->ship_controller,
                               state->graphics_context->screen_center,
                               state->ship.scale);
      } else {
        return PROGRESS;  // Game over
      }
    }

    // Render HUD
    render_hud(&state->game_hud, state->ship.scale);

    render_frame(state->graphics_context);

    // Handle events and input
    event_t event = poll_event();
    if (event == QUIT_EVENT) {
      break;
    }

    if (is_left_key_pressed(&state->game->keyboard_state)) {
      ship_controller_handle_rotate_left(&state->ship_controller);
    }

    if (is_right_key_pressed(&state->game->keyboard_state)) {
      ship_controller_handle_rotate_right(&state->ship_controller);
    }

    if (is_up_key_pressed(&state->game->keyboard_state)) {
      ship_controller_handle_thrust(&state->ship_controller);
    }

    if (is_space_key_pressed(&state->game->keyboard_state)) {
      ship_controller_handle_fire(&state->ship_controller);
    }

    if (is_f11_key_pressed(&state->game->keyboard_state)) {
      toggle_fullscreen(state->graphics_context);
    }

    if (is_esc_key_pressed(&state->game->keyboard_state)) {
      break;
    }
  }
  return QUIT;
}

/* ---- ==== Stage Interface Implementation ==== ---- */

static void playing_init(stage_ptr stage, game_ptr game) {
  playing_stage_state_ptr state = create_playing_stage(game);
  stage->state = state;
}

static game_stage_action_t playing_update(stage_ptr stage) {
  playing_stage_state_ptr state = (playing_stage_state_ptr)stage->state;
  return handle_playing_stage(state);
}

static void playing_cleanup(stage_ptr stage) {
  playing_stage_state_ptr state = (playing_stage_state_ptr)stage->state;
  destroy_playing_stage(state);
  stage->state = NULL;
}

stage_ptr create_playing_stage_instance(void) {
  stage_ptr stage = malloc(sizeof(stage_t));
  stage->state = NULL;
  stage->init = playing_init;
  stage->update = playing_update;
  stage->cleanup = playing_cleanup;
  stage->name = "PLAYING";
  return stage;
}
