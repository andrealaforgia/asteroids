#ifndef GAME_SRC_MANAGERS_ASTEROID_MANAGER_H_
#define GAME_SRC_MANAGERS_ASTEROID_MANAGER_H_

#include <stddef.h>

#include "asteroid.h"
#include "audio.h"
#include "event_system.h"
#include "game.h"
#include "geometry.h"
#include "graphics.h"
#include "object_pool.h"
#include "sharpnel.h"

typedef struct {
  graphics_context_ptr graphics_context;
  audio_context_ptr audio_context;
  game_ptr game;
  sharpnel_system_ptr sharpnel_system;
  event_system_ptr event_system;
  object_pool_t pool;
} asteroid_manager_t;

typedef asteroid_manager_t* asteroid_manager_ptr;

// Initialize the asteroid manager with required context
void init_asteroid_manager(asteroid_manager_ptr manager, game_ptr game,
                            graphics_context_ptr graphics_context,
                            audio_context_ptr audio_context,
                            sharpnel_system_ptr sharpnel_system,
                            event_system_ptr event_system);

// Reset asteroid state
void reset_asteroids(asteroid_manager_ptr manager);

// Create initial asteroids around a safe position (typically the ship)
void create_asteroids(asteroid_manager_ptr manager, point_t safe_position);

// Add a new asteroid at position with given scale
void add_asteroid(asteroid_manager_ptr manager, point_t position, int scale);

// Remove asteroid at given index
void remove_asteroid(asteroid_manager_ptr manager, size_t asteroid_index);

// Update and render all asteroids
void update_asteroids(asteroid_manager_ptr manager, double delta_time);

// Break asteroid apart (play sound, score, spawn smaller ones)
void break_asteroid_apart(asteroid_manager_ptr manager, size_t asteroid_index);

// Recreate asteroids if none are left
void recreate_asteroids_if_none_are_left(asteroid_manager_ptr manager,
                                         point_t safe_position);

// Get asteroid count
size_t get_asteroid_count(const asteroid_manager_ptr manager);

// Get asteroid at index (for collision detection)
asteroid_ptr get_asteroid(asteroid_manager_ptr manager, size_t asteroid_index);

// Get asteroid radius at index
int get_asteroid_radius(const asteroid_manager_ptr manager,
                        size_t asteroid_index);

#endif  // GAME_SRC_MANAGERS_ASTEROID_MANAGER_H_
