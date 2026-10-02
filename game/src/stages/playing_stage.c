#include "playing_stage.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "asteroid_manager.h"
#include "bullet_manager.h"
#include "clock.h"
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
#include "sharpnel.h"
#include "ship.h"
#include "ship_controller.h"
#include "sprites.h"
#include "stage.h"
#include "text.h"

/* ---- ==== ---- ==== helper functions ==== ---- ==== ---- */

static inline bool any_ship_lives_left(const playing_stage_state_ptr state) {
  return state->game->lives > 0;
}

static inline void consume_one_ship_life(playing_stage_state_ptr state) {
  --state->game->lives;
}

static inline void render_sound_notification(playing_stage_state_ptr state) {
  if (!state->sound_notification_active) {
    return;
  }

  int elapsed = elapsed_from(state->sound_notification_start_ticks);

  // Check if notification should be dismissed
  if (elapsed >= SOUND_NOTIFICATION_DURATION_MS) {
    state->sound_notification_active = false;
    return;
  }

  // Calculate fade (1.0 at start, 0.0 at end)
  double fade = 1.0 - (elapsed / (double)SOUND_NOTIFICATION_DURATION_MS);

  // Create white color with fade
  int intensity = (int)(255 * fade);
  color_t text_color = COLOR(intensity, intensity, intensity);

  const char* message =
      state->sound_notification_is_on ? "SOUND IS ON" : "SOUND IS OFF";
  int text_scale = (state->graphics_context->screen_height * 10) / 900;
  text_dimensions_t text_dimensions =
      calculate_text_dimensions(message, text_scale);

  point_t position = point(
      state->graphics_context->screen_center.x - text_dimensions.width / 2,
      state->graphics_context->screen_center.y);

  write_text(state->graphics_context, message, position, text_scale,
             text_color);
}

/* ---- ==== ---- ==== init ==== ---- ==== ---- */

static inline void create_first_ship(playing_stage_state_ptr state) {
  state->ship = create_ship(state->graphics_context->screen_center, 1);
}

static void reset_objects(playing_stage_state_ptr state) {
  reset_asteroids(&state->asteroid_manager);
  reset_sharpnels(state->sharpnel_system);
  reset_bullets(&state->bullet_manager);
  reset_saucer(&state->saucer_manager);
  reset_game_hud(&state->game_hud);
}

playing_stage_state_ptr create_playing_stage(game_ptr game) {
  playing_stage_state_ptr state = calloc(1, sizeof(playing_stage_state_t));
  if (!state) {
    return NULL;
  }
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
  if (!state->sharpnel_system) {
    destroy_playing_stage(state);
    return NULL;
  }
  init_asteroid_manager(&state->asteroid_manager, game, state->graphics_context,
                        state->audio_context, state->sharpnel_system,
                        &state->event_system);
  init_bullet_manager(&state->bullet_manager, game, state->graphics_context,
                      state->audio_context);
  if (!state->asteroid_manager.pool.objects ||
      !state->bullet_manager.ship_bullet_pool.objects ||
      !state->bullet_manager.saucer_bullet_pool.objects) {
    destroy_playing_stage(state);
    return NULL;
  }
  init_saucer_manager(&state->saucer_manager, game, state->graphics_context,
                      state->audio_context, &state->bullet_manager,
                      state->sharpnel_system, &state->event_system);
  init_game_hud(&state->game_hud, game, state->graphics_context);

  state->ship_controller = create_ship_controller(
      &state->ship, state->graphics_context, state->audio_context,
      &state->bullet_manager, state->sharpnel_system, &state->event_system);

  // Initialize sound notification state
  state->sound_notification_active = false;
  state->sound_notification_is_on = false;
  state->sound_notification_start_ticks = 0;

  create_first_ship(state);
  reset_objects(state);
  return state;
}

void destroy_playing_stage(playing_stage_state_ptr state) {
  if (state != NULL) {
    destroy_asteroid_manager(&state->asteroid_manager);
    destroy_bullet_manager(&state->bullet_manager);
    destroy_event_system(&state->event_system);
    destroy_sharpnel_system(state->sharpnel_system);
    free(state);
  }
}

/* ---- ==== ---- ==== main game loop ==== ---- ==== ---- */

static bool simulate_playing_step(playing_stage_state_ptr state,
                                  ship_input_t input, double delta_time) {
  recreate_asteroids_if_none_are_left(&state->asteroid_manager,
                                      state->ship.position);
  ship_controller_apply_input(&state->ship_controller, input, delta_time);
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
  if (check_asteroid_ship_collisions(&state->asteroid_manager, &state->ship)) {
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
      return false;
    }
  }

  return true;
}

bool advance_playing_stage(playing_stage_state_ptr state, ship_input_t input,
                           double delta_time) {
  if (!isfinite(delta_time) || delta_time <= 0) {
    return true;
  }
  // Bound catch-up after a stall, and resolve collisions at 240 Hz.
  const double step = PHYSICS_BASELINE_FPS / SIMULATION_STEPS_PER_SECOND;
  double max_delta = MAX_FRAME_CATCH_UP_MS * PHYSICS_BASELINE_FPS / 1000.0;
  state->simulation_accumulator += fmin(delta_time, max_delta);
  while (state->simulation_accumulator + 1e-9 >= step) {
    state->simulation_accumulator =
        fmax(0, state->simulation_accumulator - step);
    if (!simulate_playing_step(state, input, step)) {
      return false;
    }
  }
  return true;
}

static void render_playing_stage(playing_stage_state_ptr state) {
  clear_frame(state->graphics_context);
  render_asteroids(&state->asteroid_manager);
  render_ship_controller(&state->ship_controller);
  render_ship_bullets(&state->bullet_manager);
  if (is_saucer_flying(&state->saucer_manager)) {
    render_saucer(state->graphics_context, &state->saucer_manager.saucer);
  }
  render_saucer_bullets(&state->bullet_manager);
  render_sharpnels(state->sharpnel_system);
  render_hud(&state->game_hud, state->ship.scale);
  render_sound_notification(state);
  render_frame(state->graphics_context);
}

game_stage_action_t handle_playing_stage(playing_stage_state_ptr state) {
  frame_limiter_t limiter = create_frame_limiter(state->game->settings.fps);
  while (true) {
    double delta_time = frame_limiter_wait(&limiter);
    if (drain_events() == QUIT_EVENT) {
      return QUIT;
    }
    keyboard_state_ptr keyboard = &state->game->keyboard_state;
    if (is_esc_key_pressed(keyboard)) {
      return QUIT;
    }
    if (is_f11_key_pressed(keyboard)) {
      toggle_fullscreen(state->graphics_context);
    }
    if (is_s_key_pressed(keyboard)) {
      state->sound_notification_is_on = toggle_game_sound(state->game);
      state->sound_notification_active = true;
      state->sound_notification_start_ticks = get_clock_ticks_ms();
    }
    ship_input_t input = {.left = is_left_key_held(keyboard),
                          .right = is_right_key_held(keyboard),
                          .thrust = is_up_key_held(keyboard),
                          .fire = keyboard->keys[SDL_SCANCODE_SPACE] != 0};
    if (!advance_playing_stage(state, input, delta_time)) {
      return PROGRESS;
    }
    render_playing_stage(state);
  }
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
  if (!stage) {
    return NULL;
  }
  stage->state = NULL;
  stage->init = playing_init;
  stage->update = playing_update;
  stage->cleanup = playing_cleanup;
  stage->name = "PLAYING";
  return stage;
}
