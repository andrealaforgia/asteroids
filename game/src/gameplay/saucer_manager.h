#ifndef GAME_SRC_GAMEPLAY_SAUCER_MANAGER_H_
#define GAME_SRC_GAMEPLAY_SAUCER_MANAGER_H_

#include "audio.h"
#include "bullet_manager.h"
#include "game.h"
#include "geometry.h"
#include "graphics.h"
#include "saucer.h"

typedef struct {
  graphics_context_ptr graphics_context;
  audio_context_ptr audio_context;
  game_ptr game;
  bullet_manager_ptr bullet_manager;
  saucer_t saucer;
  int last_travel_duration_msecs;
  int last_travel_start_ticks;
  int last_bullet_fired_ticks;
} saucer_manager_t;

typedef saucer_manager_t* saucer_manager_ptr;

// Initialize the saucer manager with required context
void init_saucer_manager(saucer_manager_ptr manager, game_ptr game,
                         graphics_context_ptr graphics_context,
                         audio_context_ptr audio_context,
                         bullet_manager_ptr bullet_manager);

// Reset saucer state
void reset_saucer(saucer_manager_ptr manager);

// Create saucer if enough time has passed
void create_saucer_if_required(saucer_manager_ptr manager);

// Update saucer position and behavior
void update_saucer(saucer_manager_ptr manager, double delta_time,
                   point_t target_position);

// Destroy saucer (play sound, create sharpnel)
void destroy_saucer(saucer_manager_ptr manager);

// Check if saucer is currently flying
bool is_saucer_flying(const saucer_manager_ptr manager);

// Get saucer radius
int get_saucer_radius(const saucer_manager_ptr manager);

// Get saucer position
point_t get_saucer_position(const saucer_manager_ptr manager);

// Check if saucer is big
bool is_saucer_big(const saucer_manager_ptr manager);

#endif  // GAME_SRC_GAMEPLAY_SAUCER_MANAGER_H_
