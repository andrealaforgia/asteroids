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
