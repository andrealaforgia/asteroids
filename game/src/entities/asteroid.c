#include "asteroid.h"

#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

#include "clock.h"
#include "geometry.h"
#include "inline.h"
#include "physics.h"

ALWAYS_INLINE asteroid_t create_asteroid(point_t position, int scale,
                                         color_t color) {
  asteroid_t asteroid;
  asteroid.type = rand() % 4;
  asteroid.position = position;
  asteroid.velocity = random_velocity();
  asteroid.scale = scale;
  asteroid.color = color;
  return asteroid;
}

ALWAYS_INLINE bool scale_down(const asteroid_ptr asteroid) {
  switch (asteroid->scale) {
    case LARGE_ASTEROID_SCALE:
      asteroid->scale = MEDIUM_ASTEROID_SCALE;
      asteroid->velocity.speed *= 1.25;
      break;
    case MEDIUM_ASTEROID_SCALE:
      asteroid->scale = SMALL_ASTEROID_SCALE;
      asteroid->velocity.speed += 1.25;
      break;
    case SMALL_ASTEROID_SCALE:
      return false;
  }
  return true;
}

ALWAYS_INLINE int random_asteroid_scale(void) {
  switch (rand() % 3) {
    case 0:
      return LARGE_ASTEROID_SCALE;
    case 1:
      return MEDIUM_ASTEROID_SCALE;
    default:
      return SMALL_ASTEROID_SCALE;
  }
}
