#include "physics.h"

#include "geometry.h"
#include "inline.h"
#include "math.h"
#include "stdlib.h"

ALWAYS_INLINE velocity_t velocity(const double speed,
                                  const vector_t direction) {
  velocity_t velocity;
  velocity.speed = speed;
  velocity.direction = direction;
  return velocity;
}

ALWAYS_INLINE velocity_t random_velocity(void) {
  double angle = random_angle();
  double rnd = rand() % 10 + 1;
  double speed = 2 * rnd / (rnd + 1);
  return velocity(speed, vector(cos(angle), sin(angle)));
}
