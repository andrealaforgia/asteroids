#include "bullet_manager.h"

#include <assert.h>

#include "animate.h"
#include "clock.h"
#include "color.h"
#include "physics.h"
#include "render.h"

#define MAX_SHIP_BULLET_COUNT 20
#define SHIP_BULLET_MAX_AGE_MSECS 1000

#define SAUCER_BULLET_SPEED 10
#define MAX_SAUCER_BULLET_COUNT 20
#define SAUCER_BULLET_MAX_AGE_MSECS 5000

static bullet_t ship_bullets[MAX_SHIP_BULLET_COUNT];
static size_t ship_bullet_count = 0;

static bullet_t saucer_bullets[MAX_SAUCER_BULLET_COUNT];
static size_t saucer_bullet_count = 0;

void init_bullet_manager(bullet_manager_ptr manager, game_ptr game,
                         graphics_context_ptr graphics_context,
                         audio_context_ptr audio_context) {
  manager->game = game;
  manager->graphics_context = graphics_context;
  manager->audio_context = audio_context;
}

void reset_bullets(bullet_manager_ptr manager) {
  (void)manager;  // Unused parameter
  ship_bullet_count = 0;
  saucer_bullet_count = 0;
}

/* ---- ==== ---- ==== ship bullets ==== ---- ==== ---- */

void add_ship_bullet(bullet_manager_ptr manager, point_t position,
                     velocity_t velocity) {
  (void)manager;  // Unused parameter
  assert(ship_bullet_count < MAX_SHIP_BULLET_COUNT);
  ship_bullets[ship_bullet_count++] = create_bullet(position, velocity);
}

void remove_ship_bullet(bullet_manager_ptr manager, size_t bullet_index) {
  (void)manager;  // Unused parameter
  assert(ship_bullet_count > 0);
  if (ship_bullet_count > 1) {
    ship_bullets[bullet_index] = ship_bullets[ship_bullet_count - 1];
  }
  --ship_bullet_count;
}

static ALWAYS_INLINE void update_ship_bullet(bullet_manager_ptr manager,
                                              size_t bullet_index,
                                              double delta_time) {
  bullet_ptr bullet = &ship_bullets[bullet_index];
  int bullet_age = elapsed_from(bullet->creation_ticks);
  if (elapsed_from(bullet->creation_ticks) > SHIP_BULLET_MAX_AGE_MSECS) {
    remove_ship_bullet(manager, bullet_index);
    return;
  }
  wrap_animate(manager->graphics_context, &bullet->position, &bullet->velocity,
               delta_time);
  color_t color = GRAY_SCALE(bullet_age, SHIP_BULLET_MAX_AGE_MSECS);
  render_bullet(manager->graphics_context, bullet, color);
}

void update_ship_bullets(bullet_manager_ptr manager, double delta_time) {
  // Iterate backwards to handle removals safely
  for (int sbi = ship_bullet_count - 1; sbi >= 0; sbi--) {
    update_ship_bullet(manager, sbi, delta_time);
  }
}

size_t get_ship_bullet_count(const bullet_manager_ptr manager) {
  (void)manager;  // Unused parameter
  return ship_bullet_count;
}

bullet_ptr get_ship_bullet(bullet_manager_ptr manager, size_t bullet_index) {
  (void)manager;  // Unused parameter
  assert(bullet_index < ship_bullet_count);
  return &ship_bullets[bullet_index];
}

/* ---- ==== ---- ==== saucer bullets ==== ---- ==== ---- */

void add_saucer_bullet(bullet_manager_ptr manager, point_t position,
                       point_t target_position) {
  (void)manager;  // Unused parameter
  assert(saucer_bullet_count < MAX_SAUCER_BULLET_COUNT);
  point_t target_point = random_point_around(&target_position, 5, 10);
  velocity_t saucer_bullet_velocity =
      velocity(SAUCER_BULLET_SPEED, points_vector(&position, &target_point));
  saucer_bullets[saucer_bullet_count++] =
      create_bullet(position, saucer_bullet_velocity);
}

void remove_saucer_bullet(bullet_manager_ptr manager, size_t bullet_index) {
  (void)manager;  // Unused parameter
  assert(saucer_bullet_count > 0);
  if (saucer_bullet_count > 1) {
    saucer_bullets[bullet_index] = saucer_bullets[saucer_bullet_count - 1];
  }
  --saucer_bullet_count;
}

static ALWAYS_INLINE void update_saucer_bullet(bullet_manager_ptr manager,
                                                size_t bullet_index,
                                                double delta_time) {
  bullet_ptr bullet = &saucer_bullets[bullet_index];
  int bullet_age = elapsed_from(bullet->creation_ticks);
  if (elapsed_from(bullet->creation_ticks) > SAUCER_BULLET_MAX_AGE_MSECS) {
    remove_saucer_bullet(manager, bullet_index);
    return;
  }
  wrap_animate(manager->graphics_context, &bullet->position, &bullet->velocity,
               delta_time);
  color_t color = GRAY_SCALE(bullet_age, SAUCER_BULLET_MAX_AGE_MSECS);
  render_bullet(manager->graphics_context, bullet, color);
}

void update_saucer_bullets(bullet_manager_ptr manager, double delta_time) {
  // Iterate backwards to handle removals safely
  for (int sbi = saucer_bullet_count - 1; sbi >= 0; sbi--) {
    update_saucer_bullet(manager, sbi, delta_time);
  }
}

size_t get_saucer_bullet_count(const bullet_manager_ptr manager) {
  (void)manager;  // Unused parameter
  return saucer_bullet_count;
}

bullet_ptr get_saucer_bullet(bullet_manager_ptr manager, size_t bullet_index) {
  (void)manager;  // Unused parameter
  assert(bullet_index < saucer_bullet_count);
  return &saucer_bullets[bullet_index];
}
