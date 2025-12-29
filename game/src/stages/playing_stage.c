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
#include "game.h"
#include "game_audio.h"
#include "game_hud.h"
#include "graphics.h"
#include "keyboard.h"
#include "physics.h"
#include "render.h"
#include "saucer_manager.h"
#include "score.h"
#include "sharpnel.h"
#include "ship.h"

static game_ptr game = NULL;
static graphics_context_ptr graphics_context = NULL;
static audio_context_ptr audio_context = NULL;

static asteroid_manager_t asteroid_manager;
static bullet_manager_t bullet_manager;
static saucer_manager_t saucer_manager;
static game_hud_t game_hud;

/* ---- ==== ---- ==== ship ==== ---- ==== ---- */

#define SHIP_THRUST_TICKS 90

static ship_t ship;

static ALWAYS_INLINE bool sound_on(void) { return game->settings.volume > 0; }

static ALWAYS_INLINE void animate_ship(double delta_time) {
  wrap_animate(graphics_context, &ship.position, &ship.velocity, delta_time);
  if (ship.thrusting &&
      elapsed_from(ship.last_thrust_ticks) > SHIP_THRUST_TICKS) {
    ship.thrusting = false;
    ship.last_thrust_ticks = get_clock_ticks_ms();
  }
}

static ALWAYS_INLINE void update_ship(double delta_time) {
  animate_ship(delta_time);
  // Flashing effect during immunity
  if (elapsed_from(ship.creation_ticks) > 3000 ||
      get_clock_ticks_ms() % 5 == 0) {
    render_ship(graphics_context, &ship);
  }
}

static ALWAYS_INLINE void handle_ship_destruction(void) {
  add_sharpnel(ship.position);
  if (sound_on()) {
    play_ship_lost(audio_context);
  }
  destroy_ship(&ship);
}

static ALWAYS_INLINE bool ship_was_destroyed(void) {
  return ship.state == DESTROYED;
}

static ALWAYS_INLINE void recreate_ship(void) {
  ship = create_ship(graphics_context->screen_center, ship.scale);
}

static ALWAYS_INLINE bool any_ship_lives_left(void) { return game->lives > 0; }

static ALWAYS_INLINE void consume_one_ship_life(void) { --game->lives; }

static ALWAYS_INLINE void thrust_ship(void) {
  accelerate_ship(&ship);
  if (sound_on()) {
    play_thrust(audio_context);
  }
}

static ALWAYS_INLINE void fire_ship_bullet(void) {
  point_t bullet_position = get_cannon_position(&ship);
  velocity_t bullet_velocity = velocity(10, ship.rotation_vector);
  add_ship_bullet(&bullet_manager, bullet_position, bullet_velocity);
  if (sound_on()) {
    play_fire(&game->audio_context);
  }
}

/* ---- ==== ---- ==== init ==== ---- ==== ---- */

static ALWAYS_INLINE void create_first_ship(void) {
  ship = create_ship(graphics_context->screen_center, 1);
}

static ALWAYS_INLINE void reset_objects(void) {
  reset_asteroids(&asteroid_manager);
  reset_sharpnels();
  reset_bullets(&bullet_manager);
  reset_saucer(&saucer_manager);
  reset_game_hud(&game_hud);
}

void init_playing_stage(const game_ptr _game) {
  game = _game;
  graphics_context = &game->graphics_context;
  audio_context = &game->audio_context;

  init_asteroid_manager(&asteroid_manager, game, graphics_context,
                        audio_context);
  init_bullet_manager(&bullet_manager, game, graphics_context, audio_context);
  init_saucer_manager(&saucer_manager, game, graphics_context, audio_context,
                      &bullet_manager);
  init_game_hud(&game_hud, game, graphics_context);
}

/* ---- ==== ---- ==== main game loop ==== ---- ==== ---- */

game_stage_action_t handle_playing_stage(void) {
  int last_frame_ticks = get_clock_ticks_ms();

  create_first_ship();
  reset_objects();

  while (true) {
    int frame_time = 1000 / game->settings.fps;
    int elapsed = elapsed_from(last_frame_ticks);
    if (elapsed < frame_time) {
      SDL_Delay(1);
      continue;
    }
    last_frame_ticks = get_clock_ticks_ms();

    double delta_time = elapsed / (1000.0 / 60.0);

    clear_frame(graphics_context);

    // Create asteroids if none are left
    recreate_asteroids_if_none_are_left(&asteroid_manager, ship.position);

    // Update all game objects
    update_asteroids(&asteroid_manager, delta_time);
    update_ship(delta_time);
    update_ship_bullets(&bullet_manager, delta_time);

    if (is_saucer_flying(&saucer_manager)) {
      update_saucer(&saucer_manager, delta_time, ship.position);
    } else {
      create_saucer_if_required(&saucer_manager);
    }

    update_saucer_bullets(&bullet_manager, delta_time);
    animate_sharpnels(graphics_context, delta_time);

    // Check all collisions
    if (check_asteroid_ship_collisions(&asteroid_manager, &ship)) {
      handle_ship_destruction();
    }

    if (check_saucer_bullet_ship_collisions(&bullet_manager, &ship)) {
      handle_ship_destruction();
    }

    collision_result_t ship_saucer_collision =
        check_ship_saucer_collision(&ship, &saucer_manager);
    if (ship_saucer_collision.ship_destroyed) {
      handle_ship_destruction();
    }
    if (ship_saucer_collision.saucer_destroyed) {
      destroy_saucer(&saucer_manager);
    }

    check_ship_bullet_asteroid_collisions(&bullet_manager, &asteroid_manager);

    if (check_ship_bullet_saucer_collisions(&bullet_manager,
                                            &saucer_manager)) {
      destroy_saucer(&saucer_manager);
      if (is_saucer_big(&saucer_manager)) {
        score_large_saucer(game);
      } else {
        score_small_saucer(game);
      }
    }

    // Handle ship destruction
    if (ship_was_destroyed()) {
      if (any_ship_lives_left()) {
        consume_one_ship_life();
        recreate_ship();
      } else {
        return PROGRESS;  // Game over
      }
    }

    // Render HUD
    render_hud(&game_hud, ship.scale);

    render_frame(graphics_context);

    // Handle events and input
    event_t event = poll_event();
    if (event == QUIT_EVENT) {
      break;
    }

    if (is_left_key_pressed(&game->keyboard_state)) {
      rotate_ship_left(&ship);
    }

    if (is_right_key_pressed(&game->keyboard_state)) {
      rotate_ship_right(&ship);
    }

    if (is_up_key_pressed(&game->keyboard_state)) {
      thrust_ship();
    }

    if (is_space_key_pressed(&game->keyboard_state)) {
      fire_ship_bullet();
    }

    if (is_f11_key_pressed(&game->keyboard_state)) {
      toggle_fullscreen(graphics_context);
    }

    if (is_esc_key_pressed(&game->keyboard_state)) {
      break;
    }
  }
  return QUIT;
}
