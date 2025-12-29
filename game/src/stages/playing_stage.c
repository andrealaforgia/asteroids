#include "playing_stage.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "animate.h"
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
#include "physics.h"
#include "render.h"
#include "saucer_manager.h"
#include "score.h"
#include "sharpnel.h"
#include "ship.h"

/* ---- ==== ---- ==== ship ==== ---- ==== ---- */

static ALWAYS_INLINE bool sound_on(const playing_stage_state_ptr state) {
  return state->game->settings.volume > 0;
}

static ALWAYS_INLINE void animate_ship(playing_stage_state_ptr state,
                                        double delta_time) {
  wrap_animate(state->graphics_context, &state->ship.position,
               &state->ship.velocity, delta_time);
  if (state->ship.thrusting &&
      elapsed_from(state->ship.last_thrust_ticks) > SHIP_THRUST_DURATION_MS) {
    state->ship.thrusting = false;
    state->ship.last_thrust_ticks = get_clock_ticks_ms();
  }
}

static ALWAYS_INLINE void update_ship(playing_stage_state_ptr state,
                                       double delta_time) {
  animate_ship(state, delta_time);
  // Flashing effect during immunity
  if (elapsed_from(state->ship.creation_ticks) > SHIP_IMMUNITY_DURATION_MS ||
      get_clock_ticks_ms() % 5 == 0) {
    render_ship(state->graphics_context, &state->ship);
  }
}

static ALWAYS_INLINE void handle_ship_destruction(playing_stage_state_ptr state) {
  add_sharpnel(state->sharpnel_system, state->ship.position);
  if (sound_on(state)) {
    play_ship_lost(state->audio_context);
  }
  destroy_ship(&state->ship);
}

static ALWAYS_INLINE bool ship_was_destroyed(const playing_stage_state_ptr state) {
  return state->ship.state == DESTROYED;
}

static ALWAYS_INLINE void recreate_ship(playing_stage_state_ptr state) {
  state->ship = create_ship(state->graphics_context->screen_center,
                            state->ship.scale);
}

static ALWAYS_INLINE bool any_ship_lives_left(const playing_stage_state_ptr state) {
  return state->game->lives > 0;
}

static ALWAYS_INLINE void consume_one_ship_life(playing_stage_state_ptr state) {
  --state->game->lives;
}

static ALWAYS_INLINE void thrust_ship(playing_stage_state_ptr state) {
  accelerate_ship(&state->ship);
  if (sound_on(state)) {
    play_thrust(state->audio_context);
  }
}

static ALWAYS_INLINE void fire_ship_bullet(playing_stage_state_ptr state) {
  point_t bullet_position = get_cannon_position(&state->ship);
  velocity_t bullet_velocity = velocity(10, state->ship.rotation_vector);
  add_ship_bullet(&state->bullet_manager, bullet_position, bullet_velocity);
  if (sound_on(state)) {
    play_fire(&state->game->audio_context);
  }
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

  state->sharpnel_system =
      create_sharpnel_system(state->graphics_context, MAX_SHARPNEL_COUNT);
  init_asteroid_manager(&state->asteroid_manager, game,
                        state->graphics_context, state->audio_context,
                        state->sharpnel_system);
  init_bullet_manager(&state->bullet_manager, game, state->graphics_context,
                      state->audio_context);
  init_saucer_manager(&state->saucer_manager, game, state->graphics_context,
                      state->audio_context, &state->bullet_manager,
                      state->sharpnel_system);
  init_game_hud(&state->game_hud, game, state->graphics_context);

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
    update_ship(state, delta_time);
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
      handle_ship_destruction(state);
    }

    if (check_saucer_bullet_ship_collisions(&state->bullet_manager,
                                            &state->ship)) {
      handle_ship_destruction(state);
    }

    collision_result_t ship_saucer_collision =
        check_ship_saucer_collision(&state->ship, &state->saucer_manager);
    if (ship_saucer_collision.ship_destroyed) {
      handle_ship_destruction(state);
    }
    if (ship_saucer_collision.saucer_destroyed) {
      destroy_saucer(&state->saucer_manager);
    }

    check_ship_bullet_asteroid_collisions(&state->bullet_manager,
                                          &state->asteroid_manager);

    if (check_ship_bullet_saucer_collisions(&state->bullet_manager,
                                            &state->saucer_manager)) {
      destroy_saucer(&state->saucer_manager);
      if (is_saucer_big(&state->saucer_manager)) {
        score_large_saucer(state->game);
      } else {
        score_small_saucer(state->game);
      }
    }

    // Handle ship destruction
    if (ship_was_destroyed(state)) {
      if (any_ship_lives_left(state)) {
        consume_one_ship_life(state);
        recreate_ship(state);
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
      rotate_ship_left(&state->ship);
    }

    if (is_right_key_pressed(&state->game->keyboard_state)) {
      rotate_ship_right(&state->ship);
    }

    if (is_up_key_pressed(&state->game->keyboard_state)) {
      thrust_ship(state);
    }

    if (is_space_key_pressed(&state->game->keyboard_state)) {
      fire_ship_bullet(state);
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
