#ifndef GAME_SRC_CONTROLLERS_SHIP_CONTROLLER_H_
#define GAME_SRC_CONTROLLERS_SHIP_CONTROLLER_H_

#include <stdbool.h>

#include "audio.h"
#include "bullet_manager.h"
#include "geometry.h"
#include "graphics.h"
#include "ship.h"
#include "sharpnel.h"

typedef struct {
  ship_ptr ship;
  graphics_context_ptr graphics_context;
  audio_context_ptr audio_context;
  bullet_manager_ptr bullet_manager;
  sharpnel_system_ptr sharpnel_system;
  int volume;
} ship_controller_t;

typedef ship_controller_t* ship_controller_ptr;

ship_controller_t create_ship_controller(
    ship_ptr ship, graphics_context_ptr graphics,
    audio_context_ptr audio, bullet_manager_ptr bullets,
    sharpnel_system_ptr sharpnel, int volume);

void ship_controller_update(ship_controller_ptr controller, double delta_time);
void ship_controller_handle_thrust(ship_controller_ptr controller);
void ship_controller_handle_fire(ship_controller_ptr controller);
void ship_controller_handle_rotate_left(ship_controller_ptr controller);
void ship_controller_handle_rotate_right(ship_controller_ptr controller);
bool ship_controller_is_destroyed(const ship_controller_ptr controller);
void ship_controller_handle_destruction(ship_controller_ptr controller);
void ship_controller_respawn(ship_controller_ptr controller, point_t position,
                            int scale);

#endif  // GAME_SRC_CONTROLLERS_SHIP_CONTROLLER_H_
