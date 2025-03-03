#include "collision.h"

#include "asteroid.h"
#include "bullet.h"
#include "inline.h"
#include "saucer.h"
#include "ship.h"

ALWAYS_INLINE int asteroid_bullet_collision(const asteroid_ptr asteroid,
                                            const bullet_ptr bullet) {
  return point_distance(&asteroid->position, &bullet->position) <
         8 * asteroid->scale;
}

ALWAYS_INLINE int saucer_ship_bullet_collision(const saucer_ptr saucer,
                                               const bullet_ptr bullet) {
  return point_distance(&saucer->position, &bullet->position) <
         8 * saucer->scale;
}

ALWAYS_INLINE int ship_bullet_collision(const ship_ptr ship,
                                        const bullet_ptr bullet) {
  return point_distance(&ship->position, &bullet->position) < 8 * ship->scale;
}

ALWAYS_INLINE int ship_saucer_collision(const ship_ptr ship,
                                        const saucer_ptr saucer) {
  return point_distance(&ship->position, &saucer->position) <
         (8 * ship->scale + 8 * saucer->scale);
}

ALWAYS_INLINE int ship_asteroid_collision(const ship_ptr ship,
                                          const asteroid_ptr asteroid) {
  return point_distance(&ship->position, &asteroid->position) <
         (8 * asteroid->scale + 11 * ship->scale);
}
