#ifndef COLLISION_H_
#define COLLISION_H_

#include "asteroid.h"
#include "bullet.h"
#include "saucer.h"
#include "ship.h"

int asteroid_bullet_collision(const asteroid_ptr asteroid,
                              const bullet_ptr bullet);

int saucer_ship_bullet_collision(const saucer_ptr saucer,
                                 const bullet_ptr bullet);

int ship_bullet_collision(const ship_ptr ship, const bullet_ptr bullet);

int ship_saucer_collision(const ship_ptr ship, const saucer_ptr saucer);

int ship_asteroid_collision(const ship_ptr ship, const asteroid_ptr asteroid);

#endif  // COLLISION_H_
