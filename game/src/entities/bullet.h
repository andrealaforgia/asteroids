/**
 * @file bullet.h
 * @brief Bullet entity definition and creation
 *
 * Defines the bullet entity with position, velocity, and creation timestamp.
 * Bullets have limited lifetime and are automatically removed after a time
 * limit to prevent indefinite movement across the screen.
 */

#ifndef GAME_SRC_ENTITIES_BULLET_H_
#define GAME_SRC_ENTITIES_BULLET_H_

#include "geometry.h"
#include "physics.h"

typedef struct {
  int creation_ticks;
  point_t position;
  velocity_t velocity;
} bullet_t, *bullet_ptr;

bullet_t create_bullet(const point_t position, const velocity_t velocity);

#endif  // GAME_SRC_ENTITIES_BULLET_H_
