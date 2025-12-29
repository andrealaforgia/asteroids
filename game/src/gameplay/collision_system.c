#include "collision_system.h"

#include "clock.h"
#include "game_constants.h"
#include "geometry.h"
#include "object_pool.h"
#include "score.h"

static ALWAYS_INLINE int ship_radius(ship_ptr ship) { return 8 * ship->scale; }

static ALWAYS_INLINE bool ship_is_immune(ship_ptr ship) {
  return elapsed_from(ship->creation_ticks) <= SHIP_IMMUNITY_DURATION_MS;
}

bool check_asteroid_ship_collisions(asteroid_manager_ptr asteroid_manager,
                                    ship_ptr ship) {
  if (ship_is_immune(ship)) {
    return false;
  }

  // Iterate through pool capacity, checking active items
  for (int ai = asteroid_manager->pool.capacity - 1; ai >= 0; ai--) {
    if (!pool_is_active(&asteroid_manager->pool, ai)) {
      continue;
    }
    asteroid_ptr asteroid = get_asteroid(asteroid_manager, ai);
    if (asteroid == NULL) {
      continue;
    }
    int asteroid_radius = get_asteroid_radius(asteroid_manager, ai);
    if (point_distance(&asteroid->position, &ship->position) <
        (asteroid_radius + ship_radius(ship))) {
      break_asteroid_apart(asteroid_manager, ai);
      return true;
    }
  }
  return false;
}

void check_ship_bullet_asteroid_collisions(
    bullet_manager_ptr bullet_manager, asteroid_manager_ptr asteroid_manager) {
  // Iterate through bullet pool capacity
  for (int sbi = bullet_manager->ship_bullet_pool.capacity - 1; sbi >= 0;
       sbi--) {
    if (!pool_is_active(&bullet_manager->ship_bullet_pool, sbi)) {
      continue;
    }
    bullet_ptr bullet = get_ship_bullet(bullet_manager, sbi);
    if (bullet == NULL) {
      continue;
    }

    // Iterate through asteroid pool capacity
    for (int ai = asteroid_manager->pool.capacity - 1; ai >= 0; ai--) {
      if (!pool_is_active(&asteroid_manager->pool, ai)) {
        continue;
      }
      asteroid_ptr asteroid = get_asteroid(asteroid_manager, ai);
      if (asteroid == NULL) {
        continue;
      }
      int asteroid_radius = get_asteroid_radius(asteroid_manager, ai);
      if (point_distance(&asteroid->position, &bullet->position) <
          asteroid_radius) {
        remove_ship_bullet(bullet_manager, sbi);
        break_asteroid_apart(asteroid_manager, ai);
        break;
      }
    }
  }
}

bool check_saucer_bullet_ship_collisions(bullet_manager_ptr bullet_manager,
                                         ship_ptr ship) {
  if (ship_is_immune(ship)) {
    return false;
  }

  // Iterate through saucer bullet pool capacity
  for (int sbi = bullet_manager->saucer_bullet_pool.capacity - 1; sbi >= 0;
       sbi--) {
    if (!pool_is_active(&bullet_manager->saucer_bullet_pool, sbi)) {
      continue;
    }
    bullet_ptr bullet = get_saucer_bullet(bullet_manager, sbi);
    if (bullet == NULL) {
      continue;
    }
    if (point_distance(&ship->position, &bullet->position) <
        ship_radius(ship)) {
      remove_saucer_bullet(bullet_manager, sbi);
      return true;
    }
  }
  return false;
}

bool check_ship_bullet_saucer_collisions(bullet_manager_ptr bullet_manager,
                                         saucer_manager_ptr saucer_manager) {
  if (!is_saucer_flying(saucer_manager)) {
    return false;
  }

  point_t saucer_position = get_saucer_position(saucer_manager);
  int saucer_radius = get_saucer_radius(saucer_manager);

  // Iterate through ship bullet pool capacity
  for (int sbi = bullet_manager->ship_bullet_pool.capacity - 1; sbi >= 0;
       sbi--) {
    if (!pool_is_active(&bullet_manager->ship_bullet_pool, sbi)) {
      continue;
    }
    bullet_ptr bullet = get_ship_bullet(bullet_manager, sbi);
    if (bullet == NULL) {
      continue;
    }
    if (point_distance(&saucer_position, &bullet->position) < saucer_radius) {
      remove_ship_bullet(bullet_manager, sbi);
      return true;
    }
  }
  return false;
}

collision_result_t check_ship_saucer_collision(
    ship_ptr ship, saucer_manager_ptr saucer_manager) {
  collision_result_t result = {false, false};

  if (!is_saucer_flying(saucer_manager) || ship_is_immune(ship)) {
    return result;
  }

  point_t saucer_position = get_saucer_position(saucer_manager);
  int saucer_radius = get_saucer_radius(saucer_manager);

  if (point_distance(&ship->position, &saucer_position) <
      (ship_radius(ship) + saucer_radius)) {
    result.ship_destroyed = true;
    result.saucer_destroyed = true;
  }

  return result;
}
