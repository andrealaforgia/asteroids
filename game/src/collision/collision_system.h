/**
 * @file collision_system.h
 * @brief High-level collision detection and resolution system
 *
 * Coordinates collision checks between all game entities including
 * asteroids, bullets, ships, and saucers. Handles collision resolution
 * by destroying entities and triggering appropriate game events and effects.
 */

#ifndef GAME_SRC_COLLISION_COLLISION_SYSTEM_H_
#define GAME_SRC_COLLISION_COLLISION_SYSTEM_H_

#include <stdbool.h>

#include "asteroid_manager.h"
#include "bullet_manager.h"
#include "saucer_manager.h"
#include "ship.h"

// Result of collision checks
typedef struct {
  bool ship_destroyed;
  bool saucer_destroyed;
} collision_result_t;

// Check if asteroid hits ship (only if ship is not immune)
// Returns true if ship was destroyed
bool check_asteroid_ship_collisions(asteroid_manager_ptr asteroid_manager,
                                    ship_ptr ship);

// Check if ship bullets hit asteroids
// Destroys both bullet and asteroid on collision
void check_ship_bullet_asteroid_collisions(
    bullet_manager_ptr bullet_manager,
    asteroid_manager_ptr asteroid_manager);

// Check if saucer bullets hit ship
// Returns true if ship was destroyed
bool check_saucer_bullet_ship_collisions(bullet_manager_ptr bullet_manager,
                                         ship_ptr ship);

// Check if ship bullets hit saucer
// Returns true if saucer was destroyed
bool check_ship_bullet_saucer_collisions(bullet_manager_ptr bullet_manager,
                                         saucer_manager_ptr saucer_manager);

// Check if ship collides with saucer
// Returns collision result indicating what was destroyed
collision_result_t check_ship_saucer_collision(
    ship_ptr ship, saucer_manager_ptr saucer_manager);

#endif  // GAME_SRC_COLLISION_COLLISION_SYSTEM_H_
