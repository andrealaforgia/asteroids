#include "bullet_manager.h"

#include <assert.h>

#include "animate.h"
#include "clock.h"
#include "color.h"
#include "game_constants.h"
#include "physics.h"
#include "sprites.h"

void init_bullet_manager(bullet_manager_ptr manager, game_ptr game,
                         graphics_context_ptr graphics_context,
                         audio_context_ptr audio_context) {
  manager->game = game;
  manager->graphics_context = graphics_context;
  manager->audio_context = audio_context;
  manager->ship_bullet_pool =
      create_object_pool(sizeof(bullet_t), MAX_SHIP_BULLET_COUNT);
  manager->saucer_bullet_pool =
      create_object_pool(sizeof(bullet_t), MAX_SAUCER_BULLET_COUNT);
}

void reset_bullets(bullet_manager_ptr manager) {
  pool_reset(&manager->ship_bullet_pool);
  pool_reset(&manager->saucer_bullet_pool);
}

/* ---- ==== ---- ==== ship bullets ==== ---- ==== ---- */

void add_ship_bullet(bullet_manager_ptr manager, point_t position,
                     velocity_t velocity) {
  size_t index;
  bullet_t* bullet = (bullet_t*)pool_acquire(&manager->ship_bullet_pool, &index);
  if (bullet == NULL) {
    return;  // Pool exhausted
  }
  *bullet = create_bullet(position, velocity);
}

void remove_ship_bullet(bullet_manager_ptr manager, size_t bullet_index) {
  pool_release(&manager->ship_bullet_pool, bullet_index);
}

void update_ship_bullets(bullet_manager_ptr manager, double delta_time) {
  // Iterate backwards through pool capacity to safely handle removals
  for (int sbi = manager->ship_bullet_pool.capacity - 1; sbi >= 0; sbi--) {
    if (!pool_is_active(&manager->ship_bullet_pool, sbi)) {
      continue;
    }

    bullet_ptr bullet = (bullet_ptr)pool_get_at(&manager->ship_bullet_pool, sbi);
    int bullet_age = elapsed_from(bullet->creation_ticks);

    if (elapsed_from(bullet->creation_ticks) > SHIP_BULLET_MAX_AGE_MS) {
      remove_ship_bullet(manager, sbi);
      continue;
    }

    wrap_animate(manager->graphics_context, &bullet->position,
                 &bullet->velocity, delta_time);
    color_t color = GRAY_SCALE(bullet_age, SHIP_BULLET_MAX_AGE_MS);
    render_bullet(manager->graphics_context, bullet, color);
  }
}

size_t get_ship_bullet_count(const bullet_manager_ptr manager) {
  return pool_get_active_count(&manager->ship_bullet_pool);
}

bullet_ptr get_ship_bullet(bullet_manager_ptr manager, size_t bullet_index) {
  return (bullet_ptr)pool_get_at(&manager->ship_bullet_pool, bullet_index);
}

/* ---- ==== ---- ==== saucer bullets ==== ---- ==== ---- */

void add_saucer_bullet(bullet_manager_ptr manager, point_t position,
                       point_t target_position) {
  size_t index;
  bullet_t* bullet =
      (bullet_t*)pool_acquire(&manager->saucer_bullet_pool, &index);
  if (bullet == NULL) {
    return;  // Pool exhausted
  }

  point_t target_point = random_point_around(&target_position, 5, 10);
  velocity_t saucer_bullet_velocity =
      velocity(SAUCER_BULLET_SPEED, points_vector(&position, &target_point));
  *bullet = create_bullet(position, saucer_bullet_velocity);
}

void remove_saucer_bullet(bullet_manager_ptr manager, size_t bullet_index) {
  pool_release(&manager->saucer_bullet_pool, bullet_index);
}

void update_saucer_bullets(bullet_manager_ptr manager, double delta_time) {
  // Iterate backwards through pool capacity to safely handle removals
  for (int sbi = manager->saucer_bullet_pool.capacity - 1; sbi >= 0; sbi--) {
    if (!pool_is_active(&manager->saucer_bullet_pool, sbi)) {
      continue;
    }

    bullet_ptr bullet =
        (bullet_ptr)pool_get_at(&manager->saucer_bullet_pool, sbi);
    int bullet_age = elapsed_from(bullet->creation_ticks);

    if (elapsed_from(bullet->creation_ticks) > SAUCER_BULLET_MAX_AGE_MS) {
      remove_saucer_bullet(manager, sbi);
      continue;
    }

    wrap_animate(manager->graphics_context, &bullet->position,
                 &bullet->velocity, delta_time);
    color_t color = GRAY_SCALE(bullet_age, SAUCER_BULLET_MAX_AGE_MS);
    render_bullet(manager->graphics_context, bullet, color);
  }
}

size_t get_saucer_bullet_count(const bullet_manager_ptr manager) {
  return pool_get_active_count(&manager->saucer_bullet_pool);
}

bullet_ptr get_saucer_bullet(bullet_manager_ptr manager, size_t bullet_index) {
  return (bullet_ptr)pool_get_at(&manager->saucer_bullet_pool, bullet_index);
}
