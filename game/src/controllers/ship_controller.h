/**
 * @file ship_controller.h
 * @brief Player ship control and behavior management
 *
 * Manages player ship controls including thrust, rotation, and firing.
 * Handles ship state updates, destruction sequences, respawning, and
 * coordinates with bullet manager and audio system for player actions.
 */

#ifndef GAME_SRC_CONTROLLERS_SHIP_CONTROLLER_H_
#define GAME_SRC_CONTROLLERS_SHIP_CONTROLLER_H_

#include <stdbool.h>

#include "audio.h"
#include "bullet_manager.h"
#include "event_system.h"
#include "geometry.h"
#include "graphics.h"
#include "sharpnel.h"
#include "ship.h"

typedef struct {
  ship_ptr ship;
  graphics_context_ptr graphics_context;
  audio_context_ptr audio_context;
  bullet_manager_ptr bullet_manager;
  sharpnel_system_ptr sharpnel_system;
  event_system_ptr event_system;
  double rotation_remainder;
  double fire_remaining;
  double thrust_sound_remaining;
} ship_controller_t;

typedef ship_controller_t* ship_controller_ptr;

typedef struct {
  bool left;
  bool right;
  bool thrust;
  bool fire;
} ship_input_t;

void ship_controller_apply_input(ship_controller_ptr controller,
                                 ship_input_t input, double delta_time);
void render_ship_controller(ship_controller_ptr controller);

ship_controller_t create_ship_controller(ship_ptr ship,
                                         graphics_context_ptr graphics,
                                         audio_context_ptr audio,
                                         bullet_manager_ptr bullets,
                                         sharpnel_system_ptr sharpnel,
                                         event_system_ptr event_system);

void ship_controller_update(ship_controller_ptr controller, double delta_time);
void ship_controller_handle_thrust(ship_controller_ptr controller,
                                   double delta_time);
void ship_controller_handle_fire(ship_controller_ptr controller);
void ship_controller_handle_rotate_left(ship_controller_ptr controller);
void ship_controller_handle_rotate_right(ship_controller_ptr controller);
bool ship_controller_is_destroyed(const ship_controller_ptr controller);
void ship_controller_handle_destruction(ship_controller_ptr controller);
void ship_controller_respawn(ship_controller_ptr controller, point_t position,
                             int scale);

#endif  // GAME_SRC_CONTROLLERS_SHIP_CONTROLLER_H_
