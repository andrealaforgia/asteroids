#include "playing_stage.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "animate.h"
#include "asteroid.h"
#include "audio.h"
#include "bullet.h"
#include "clock.h"
#include "color.h"
#include "events.h"
#include "fps_tracker.h"
#include "frame.h"
#include "game.h"
#include "geometry.h"
#include "graphics.h"
#include "physics.h"
#include "render.h"
#include "saucer.h"
#include "score.h"
#include "sharpnel.h"
#include "text.h"

static game_ptr game = NULL;
static graphics_context_ptr graphics_context = NULL;
static audio_context_ptr audio_context = NULL;
static fps_tracker_t fps_tracker;

/* ---- ==== ---- ==== sound ==== ---- ==== ---- */

static ALWAYS_INLINE bool sound_on(void) { return !game->settings.no_sound; }

static ALWAYS_INLINE void play_bang_large_if_sound_on(void) {
  if (sound_on()) {
    play_bang_large(audio_context);
  }
}

static ALWAYS_INLINE void play_bang_medium_if_sound_on(void) {
  if (sound_on()) {
    play_bang_medium(audio_context);
  }
}

static ALWAYS_INLINE void play_bang_small_if_sound_on(void) {
  if (sound_on()) {
    play_bang_small(audio_context);
  }
}

static ALWAYS_INLINE void play_thrust_if_sound_on(void) {
  if (sound_on()) {
    play_thrust(audio_context);
  }
}

static ALWAYS_INLINE void toggle_sound(void) {
  game->settings.no_sound = !game->settings.no_sound;
}

/* ---- ==== ---- ==== ship ==== ---- ==== ---- */

#define SHIP_THRUST_TICKS 90
#define SHIP_IMMUNITY_DURATION_MSECS 3000

static ship_t ship;

static ALWAYS_INLINE int ship_radius(void) { return 8 * ship.scale; }

static ALWAYS_INLINE bool ship_is_not_immune(void) {
  return elapsed_from(ship.creation_ticks) > SHIP_IMMUNITY_DURATION_MSECS;
}

static ALWAYS_INLINE void animate_ship(void) {
  wrap_animate(graphics_context, &ship.position, &ship.velocity);
  if (ship.thrusting &&
      elapsed_from(ship.last_thrust_ticks) > SHIP_THRUST_TICKS) {
    ship.thrusting = false;
    ship.last_thrust_ticks = get_clock_ticks_ms();
  }
}

static ALWAYS_INLINE void update_ship(void) {
  animate_ship();
  if (ship_is_not_immune() || get_clock_ticks_ms() % 5 == 0) {
    render_ship(graphics_context, &ship);
  }
}

static ALWAYS_INLINE void handle_ship_destruction(void) {
  add_sharpnel(ship.position);
  play_bang_small_if_sound_on();
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
  play_thrust_if_sound_on();
}

/* ---- ==== ---- ==== asteroids ==== ---- ==== ---- */

#define MAX_ASTEROID_COUNT 1000

static asteroid_t asteroids[MAX_ASTEROID_COUNT];
static size_t asteroid_count = 0;

static ALWAYS_INLINE void add_asteroid(point_t position, int scale) {
  assert(asteroid_count < MAX_ASTEROID_COUNT);
  asteroids[asteroid_count++] =
      create_asteroid(position, scale, random_color());
}

static ALWAYS_INLINE void remove_asteroid(size_t asteroid_index) {
  assert(asteroid_count > 0);
  if (asteroid_count > 1) {
    asteroids[asteroid_index] = asteroids[asteroid_count - 1];
  }
  --asteroid_count;
}

static ALWAYS_INLINE void animate_asteroid(size_t asteroid_index) {
  asteroid_ptr asteroid = &asteroids[asteroid_index];
  wrap_animate(graphics_context, &asteroid->position, &asteroid->velocity);
}

static ALWAYS_INLINE void update_asteroid(size_t asteroid_index) {
  animate_asteroid(asteroid_index);
  render_asteroid(graphics_context, &asteroids[asteroid_index]);
}

static ALWAYS_INLINE int asteroid_radius(size_t asteroid_index) {
  return 8 * asteroids[asteroid_index].scale;
}

static ALWAYS_INLINE void break_asteroid_apart(size_t asteroid_index) {
  asteroid_ptr asteroid = &asteroids[asteroid_index];
  add_sharpnel(asteroid->position);
  switch (asteroid->scale) {
    case LARGE_ASTEROID_SCALE: {
      play_bang_large_if_sound_on();
      score_large_asteroid(game);
      break;
    }
    case MEDIUM_ASTEROID_SCALE: {
      play_bang_medium_if_sound_on();
      score_medium_asteroid(game);
      break;
    }
    case SMALL_ASTEROID_SCALE: {
      play_bang_small_if_sound_on();
      score_small_asteroid(game);
      break;
    }
  }
  if (scale_down(asteroid)) {
    add_asteroid(asteroid->position, asteroid->scale);
  } else {
    remove_asteroid(asteroid_index);
  }
}

/* ---- ==== ---- ==== ship bullets ==== ---- ==== ---- */

#define MAX_SHIP_BULLET_COUNT 20
#define SHIP_BULLET_MAX_AGE_MSECS 1000

static bullet_t ship_bullets[MAX_SHIP_BULLET_COUNT];
static size_t ship_bullet_count = 0;

static ALWAYS_INLINE void remove_ship_bullet(size_t bullet_index) {
  assert(ship_bullet_count > 0);
  if (ship_bullet_count > 1) {
    ship_bullets[bullet_index] = ship_bullets[ship_bullet_count - 1];
  }
  --ship_bullet_count;
}

static ALWAYS_INLINE void update_ship_bullet(size_t bullet_index) {
  bullet_ptr bullet = &ship_bullets[bullet_index];
  int bullet_age = elapsed_from(bullet->creation_ticks);
  if (elapsed_from(bullet->creation_ticks) > SHIP_BULLET_MAX_AGE_MSECS) {
    remove_ship_bullet(bullet_index);
    return;
  }
  wrap_animate(graphics_context, &bullet->position, &bullet->velocity);
  color_t color = GRAY_SCALE(bullet_age, SHIP_BULLET_MAX_AGE_MSECS);
  render_bullet(graphics_context, bullet, color);
}

static ALWAYS_INLINE void add_ship_bullet(point_t position,
                                          velocity_t velocity) {
  assert(ship_bullet_count < MAX_SHIP_BULLET_COUNT);
  ship_bullets[ship_bullet_count++] = create_bullet(position, velocity);
}

static ALWAYS_INLINE void fire_ship_bullet(void) {
  point_t bullet_position = get_cannon_position(&ship);
  velocity_t bullet_velocity = velocity(10, ship.rotation_vector);
  add_ship_bullet(bullet_position, bullet_velocity);
  if (sound_on()) {
    play_fire(&game->audio_context);
  }
}

static ALWAYS_INLINE void animate_ship_bullets(void) {
  for (size_t sbi = 0; sbi < ship_bullet_count; sbi++) {
    update_ship_bullet(sbi);
  }
}

/* ---- ==== ---- ==== saucer bullets ==== ---- ==== ---- */

#define SAUCER_BULLET_SPEED 10
#define MAX_SAUCER_BULLET_COUNT 20
#define SAUCER_BULLET_MAX_AGE_MSECS 5000

static bullet_t saucer_bullets[MAX_SAUCER_BULLET_COUNT];
static size_t saucer_bullet_count = 0;

static ALWAYS_INLINE void add_saucer_bullet(point_t position) {
  assert(saucer_bullet_count < MAX_SAUCER_BULLET_COUNT);
  point_t target_point = random_point_around(&ship.position, 5, 10);
  velocity_t saucer_bullet_velocity =
      velocity(SAUCER_BULLET_SPEED, points_vector(&position, &target_point));
  saucer_bullets[saucer_bullet_count++] =
      create_bullet(position, saucer_bullet_velocity);
}

static ALWAYS_INLINE void remove_saucer_bullet(size_t bullet_index) {
  assert(saucer_bullet_count > 0);
  if (saucer_bullet_count > 1) {
    ship_bullets[bullet_index] = ship_bullets[saucer_bullet_count - 1];
  }
  --saucer_bullet_count;
}

static ALWAYS_INLINE void update_saucer_bullet(size_t bullet_index) {
  bullet_ptr bullet = &saucer_bullets[bullet_index];
  int bullet_age = elapsed_from(bullet->creation_ticks);
  if (elapsed_from(bullet->creation_ticks) > SAUCER_BULLET_MAX_AGE_MSECS) {
    remove_saucer_bullet(bullet_index);
    return;
  }
  wrap_animate(graphics_context, &bullet->position, &bullet->velocity);
  color_t color = GRAY_SCALE(bullet_age, SAUCER_BULLET_MAX_AGE_MSECS);
  render_bullet(graphics_context, bullet, color);
}

static ALWAYS_INLINE void animate_saucer_bullets(void) {
  for (size_t sbi = 0; sbi < saucer_bullet_count; sbi++) {
    update_saucer_bullet(sbi);
  }
}

/* ---- ==== ---- ==== saucer ==== ---- ==== ---- */

#define SAUCER_CREATION_FREQUENCY_MSECS 30000
#define MAX_SAUCER_BULLET_FIRE_INTERVAL_MSECS 3000

static saucer_t saucer;
static int saucer_last_travel_duration_msecs = 0;
static int saucer_last_travel_start_ticks = 0;
static int saucer_last_bullet_fired_ticks = 0;

static ALWAYS_INLINE int saucer_radius(void) { return 8 * saucer.scale; }

static ALWAYS_INLINE bool saucer_is_flying(void) { return saucer.flying; }

static ALWAYS_INLINE void create_saucer_if_required(void) {
  int wait_time =
      SAUCER_CREATION_FREQUENCY_MSECS + saucer_last_travel_duration_msecs;
  if (elapsed_from(saucer_last_travel_start_ticks) > wait_time) {
    saucer = create_saucer(graphics_context);
    saucer_last_travel_start_ticks = get_clock_ticks_ms();
  }
}

static ALWAYS_INLINE void fire_saucer_bullet(void) {
  add_saucer_bullet(saucer.position);
}

static ALWAYS_INLINE void update_saucer(void) {
  animate(&saucer.position, &saucer.velocity);
  if (out_of_bounds(graphics_context, &saucer.position)) {
    saucer.flying = false;
    saucer_last_travel_duration_msecs =
        elapsed_from(saucer_last_travel_start_ticks);
  } else {
    render_saucer(graphics_context, &saucer);
    if (elapsed_from(saucer_last_bullet_fired_ticks) >
        MAX_SAUCER_BULLET_FIRE_INTERVAL_MSECS) {
      fire_saucer_bullet();
      saucer_last_bullet_fired_ticks = get_clock_ticks_ms();
    }
  }
}

static ALWAYS_INLINE void destroy_saucer(void) {
  saucer.flying = false;
  add_sharpnel(saucer.position);
  if (is_big(&saucer)) {
    play_bang_large_if_sound_on();
  } else {
    play_bang_small_if_sound_on();
  }
}

/* ---- ==== ---- ==== collisions ==== ---- ==== ---- */

static ALWAYS_INLINE bool asteroid_and_ship_collide(size_t asteroid_index) {
  asteroid_ptr asteroid = &asteroids[asteroid_index];
  return point_distance(&asteroid->position, &ship.position) <
         (asteroid_radius(asteroid_index) + ship_radius());
}

static ALWAYS_INLINE bool ship_and_saucer_collide(void) {
  return point_distance(&ship.position, &saucer.position) <
         (ship_radius() + saucer_radius());
}

/* ---- ==== ---- ==== ---- ==== ---- ==== ---- ==== ---- */

static ALWAYS_INLINE void show_lives(void) {
  render_lives(graphics_context,
               point(graphics_context->screen_center.x -
                         ship.scale * 12 * game->lives - 10,
                     ship.scale * 12 + 5),
               game->lives);
}

static ALWAYS_INLINE void show_score(void) {
  text_dimensions_t text_dimensions = calculate_text_dimensions("9", 10);
  write_number(
      graphics_context,
      point(graphics_context->screen_center.x, text_dimensions.height + 5),
      game->score, 10);
}

static ALWAYS_INLINE void show_fps_if_required(void) {
  track_fps(&fps_tracker);
  if (game->settings.show_fps) {
    char fps_text[10] = {0};
    format_fps(&fps_tracker, fps_text, sizeof fps_text);
    text_dimensions_t fps_text_dimensions =
        calculate_text_dimensions(fps_text, 5);
    write_text(graphics_context, fps_text,
               point(5, 5 + fps_text_dimensions.height), 5, COLOR_WHITE);
  }
}

static ALWAYS_INLINE void show_sound_status(void) {
  char *text = "SOUND ON";
  if (game->settings.no_sound) {
    text = "SOUND OFF";
  }
  text_dimensions_t text_dimensions = calculate_text_dimensions(text, 10);
  write_text(
      graphics_context, text,
      point(graphics_context->screen_center.x / 4 - text_dimensions.width / 2,
            5 + text_dimensions.height),
      10, COLOR_WHITE);
}

static ALWAYS_INLINE void check_if_asteroid_hits_ship(void) {
  for (size_t ai = 0; ai < asteroid_count; ai++) {
    if (asteroid_and_ship_collide(ai)) {
      break_asteroid_apart(ai);
      handle_ship_destruction();
      break;
    }
  }
}

static ALWAYS_INLINE bool ship_bullet_hits_asteroid(size_t ship_bullet_index,
                                                    size_t asteroid_index) {
  asteroid_ptr asteroid = &asteroids[asteroid_index];
  bullet_ptr bullet = &ship_bullets[ship_bullet_index];
  return point_distance(&asteroid->position, &bullet->position) <
         asteroid_radius(asteroid_index);
}

static ALWAYS_INLINE void check_if_ship_hits_asteroid(void) {
  for (size_t sbi = 0; sbi < ship_bullet_count; sbi++) {
    for (size_t ai = 0; ai < asteroid_count; ai++) {
      if (ship_bullet_hits_asteroid(sbi, ai)) {
        remove_ship_bullet(sbi);
        break_asteroid_apart(ai);
        break;
      }
    }
  }
}

static ALWAYS_INLINE void animate_asteroids(void) {
  for (size_t ai = 0; ai < asteroid_count; ai++) {
    update_asteroid(ai);
  }
}

static ALWAYS_INLINE bool saucer_bullet_hits_ship(size_t bullet_index) {
  bullet_ptr bullet = &saucer_bullets[bullet_index];
  return point_distance(&ship.position, &bullet->position) < ship_radius();
}

static ALWAYS_INLINE void check_if_saucer_hits_ship(void) {
  for (size_t sbi = 0; sbi < saucer_bullet_count; sbi++) {
    if (saucer_bullet_hits_ship(sbi)) {
      remove_saucer_bullet(sbi);
      handle_ship_destruction();
      break;
    }
  }
}

static ALWAYS_INLINE bool ship_bullet_hits_saucer(size_t bullet_index) {
  bullet_ptr bullet = &ship_bullets[bullet_index];
  return saucer.flying &&
         point_distance(&saucer.position, &bullet->position) < saucer_radius();
}

static ALWAYS_INLINE void check_if_ship_hits_saucer(void) {
  for (size_t sbi = 0; sbi < ship_bullet_count; sbi++) {
    if (ship_bullet_hits_saucer(sbi)) {
      remove_ship_bullet(sbi);
      destroy_saucer();
      if (is_big(&saucer)) {
        score_large_saucer(game);
      } else {
        score_small_saucer(game);
      }
      break;
    }
  }
}

static ALWAYS_INLINE void check_ship_collisions_only_if_ship_is_not_immune(
    void) {
  if (ship_is_not_immune()) {
    if (saucer_is_flying() && ship_and_saucer_collide()) {
      handle_ship_destruction();
      destroy_saucer();
    } else {
      check_if_asteroid_hits_ship();
      check_if_saucer_hits_ship();
    }
  }
}

static ALWAYS_INLINE void create_first_ship(void) {
  ship = create_ship(graphics_context->screen_center, 1);
}

static ALWAYS_INLINE void create_asteroids(void) {
  int distance_from_ship = 8 * LARGE_ASTEROID_SCALE * 4;
  size_t initial_asteroid_count = (graphics_context->screen_width * 15) / 1440;
  for (size_t i = 0; i < initial_asteroid_count; i++) {
    while (true) {
      point_t asteroid_position = random_point(graphics_context);
      if (point_distance(&asteroid_position, &ship.position) >
          distance_from_ship) {
        add_asteroid(asteroid_position, LARGE_ASTEROID_SCALE);
        break;
      }
    }
  }
}

static ALWAYS_INLINE void recreate_asteroids_if_none_are_left(void) {
  if (asteroid_count == 0) {
    create_asteroids();
  }
}

/* ---- ==== ---- ==== init ==== ---- ==== ---- */

static ALWAYS_INLINE void reset_objects(void) {
  asteroid_count = 0;
  reset_sharpnels();
  saucer_bullet_count = 0;
  ship_bullet_count = 0;
  saucer.flying = false;
  fps_tracker = create_fps_tracker();
}

void init_playing_stage(const game_ptr _game) {
  game = _game;
  graphics_context = &game->graphics_context;
  audio_context = &game->audio_context;
}

game_stage_action_t handle_playing_stage(void) {
  int last_frame_ticks = get_clock_ticks_ms();

  create_first_ship();

  reset_objects();

  while (true) {
    if (elapsed_from(last_frame_ticks) < (1000 / game->settings.fps)) {
      continue;
    }
    last_frame_ticks = get_clock_ticks_ms();

    clear_frame(graphics_context);

    recreate_asteroids_if_none_are_left();

    animate_asteroids();

    update_ship();

    check_ship_collisions_only_if_ship_is_not_immune();

    check_if_ship_hits_saucer();

    check_if_ship_hits_asteroid();

    if (ship_was_destroyed()) {
      if (any_ship_lives_left()) {
        consume_one_ship_life();
        recreate_ship();
      } else {
        return PROGRESS;  // PROGRESSES TO GAME OVER!
      }
    }

    animate_ship_bullets();

    if (saucer_is_flying()) {
      update_saucer();
    } else {
      create_saucer_if_required();
    }

    animate_saucer_bullets();

    animate_sharpnels(graphics_context);

    show_lives();

    show_score();

    show_sound_status();

    show_fps_if_required();

    render_frame(graphics_context);

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

    if (is_s_key_pressed(&game->keyboard_state)) {
      toggle_sound();
    }

    if (is_esc_key_pressed(&game->keyboard_state)) {
      break;
    }
  }
  return QUIT;
}
