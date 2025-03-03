#include "bullet.h"

#include "clock.h"
#include "geometry.h"
#include "inline.h"

ALWAYS_INLINE bullet_t create_bullet(const point_t position,
                                     const velocity_t velocity) {
  bullet_t bullet;
  bullet.position = position;
  bullet.velocity = velocity;
  bullet.creation_ticks = get_clock_ticks_ms();
  return bullet;
}
