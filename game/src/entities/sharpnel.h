/**
 * @file sharpnel.h
 * @brief Particle effect system for explosions
 *
 * Manages shrapnel particles that appear when entities are destroyed.
 * Uses an object pool for efficient particle allocation and provides
 * functions for adding and animating explosion debris.
 */

#ifndef GAME_SRC_ENTITIES_SHARPNEL_H_
#define GAME_SRC_ENTITIES_SHARPNEL_H_

#include <stdbool.h>
#include <stdlib.h>

#include "geometry.h"
#include "graphics.h"
#include "inline.h"
#include "object_pool.h"

typedef struct {
  point_t position;
  double scale;
  int creation_ticks;
} sharpnel_t, *sharpnel_ptr;

typedef struct {
  object_pool_t pool;
  graphics_context_ptr graphics_context;
} sharpnel_system_t;

typedef sharpnel_system_t* sharpnel_system_ptr;

// Create and destroy sharpnel system
sharpnel_system_t* create_sharpnel_system(graphics_context_ptr graphics_context,
                                          size_t max_count);
void destroy_sharpnel_system(sharpnel_system_t* system);

// Sharpnel operations
void reset_sharpnels(sharpnel_system_t* system);
void add_sharpnel(sharpnel_system_t* system, point_t position);
void animate_sharpnels(sharpnel_system_t* system, double delta_time);

#endif  // GAME_SRC_ENTITIES_SHARPNEL_H_
