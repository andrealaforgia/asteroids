#ifndef GAME_SRC_GAMEPLAY_BULLET_MANAGER_H_
#define GAME_SRC_GAMEPLAY_BULLET_MANAGER_H_

#include <stddef.h>

#include "audio.h"
#include "bullet.h"
#include "game.h"
#include "geometry.h"
#include "graphics.h"
#include "object_pool.h"

typedef struct {
  graphics_context_ptr graphics_context;
  audio_context_ptr audio_context;
  game_ptr game;
  object_pool_t ship_bullet_pool;
  object_pool_t saucer_bullet_pool;
} bullet_manager_t;

typedef bullet_manager_t* bullet_manager_ptr;

// Initialize the bullet manager with required context
void init_bullet_manager(bullet_manager_ptr manager, game_ptr game,
                         graphics_context_ptr graphics_context,
                         audio_context_ptr audio_context);

// Reset bullet state
void reset_bullets(bullet_manager_ptr manager);

// Ship bullet management
void add_ship_bullet(bullet_manager_ptr manager, point_t position,
                     velocity_t velocity);
void remove_ship_bullet(bullet_manager_ptr manager, size_t bullet_index);
void update_ship_bullets(bullet_manager_ptr manager, double delta_time);
size_t get_ship_bullet_count(const bullet_manager_ptr manager);
bullet_ptr get_ship_bullet(bullet_manager_ptr manager, size_t bullet_index);

// Saucer bullet management
void add_saucer_bullet(bullet_manager_ptr manager, point_t position,
                       point_t target_position);
void remove_saucer_bullet(bullet_manager_ptr manager, size_t bullet_index);
void update_saucer_bullets(bullet_manager_ptr manager, double delta_time);
size_t get_saucer_bullet_count(const bullet_manager_ptr manager);
bullet_ptr get_saucer_bullet(bullet_manager_ptr manager, size_t bullet_index);

#endif  // GAME_SRC_GAMEPLAY_BULLET_MANAGER_H_
