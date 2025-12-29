#include "ship_controller.h"

#include "animate.h"
#include "clock.h"
#include "game_audio.h"
#include "game_constants.h"
#include "game_events.h"
#include "inline.h"
#include "physics.h"
#include "sprites.h"

static ALWAYS_INLINE bool sound_on(const ship_controller_ptr controller) {
  return controller->volume > 0;
}

ship_controller_t create_ship_controller(
    ship_ptr ship, graphics_context_ptr graphics,
    audio_context_ptr audio, bullet_manager_ptr bullets,
    sharpnel_system_ptr sharpnel, event_system_ptr event_system,
    int volume) {
  ship_controller_t controller;
  controller.ship = ship;
  controller.graphics_context = graphics;
  controller.audio_context = audio;
  controller.bullet_manager = bullets;
  controller.sharpnel_system = sharpnel;
  controller.event_system = event_system;
  controller.volume = volume;
  return controller;
}

void ship_controller_update(ship_controller_ptr controller, double delta_time) {
  // Animate ship position and velocity
  wrap_animate(controller->graphics_context, &controller->ship->position,
               &controller->ship->velocity, delta_time);

  // Update thrust animation
  if (controller->ship->thrusting &&
      elapsed_from(controller->ship->last_thrust_ticks) >
          SHIP_THRUST_DURATION_MS) {
    controller->ship->thrusting = false;
    controller->ship->last_thrust_ticks = get_clock_ticks_ms();
  }

  // Render ship with flashing effect during immunity
  if (elapsed_from(controller->ship->creation_ticks) >
          SHIP_IMMUNITY_DURATION_MS ||
      get_clock_ticks_ms() % 5 == 0) {
    render_ship(controller->graphics_context, controller->ship);
  }
}

void ship_controller_handle_thrust(ship_controller_ptr controller) {
  accelerate_ship(controller->ship);
  if (sound_on(controller)) {
    play_thrust(controller->audio_context);
  }
}

void ship_controller_handle_fire(ship_controller_ptr controller) {
  point_t bullet_position = get_cannon_position(controller->ship);
  velocity_t bullet_velocity =
      velocity(10, controller->ship->rotation_vector);
  add_ship_bullet(controller->bullet_manager, bullet_position, bullet_velocity);
  if (sound_on(controller)) {
    play_fire(controller->audio_context);
  }
}

void ship_controller_handle_rotate_left(ship_controller_ptr controller) {
  rotate_ship_left(controller->ship);
}

void ship_controller_handle_rotate_right(ship_controller_ptr controller) {
  rotate_ship_right(controller->ship);
}

bool ship_controller_is_destroyed(const ship_controller_ptr controller) {
  return controller->ship->state == DESTROYED;
}

void ship_controller_handle_destruction(ship_controller_ptr controller) {
  add_sharpnel(controller->sharpnel_system, controller->ship->position);

  // Publish ship destroyed event
  ship_destroyed_data_t event_data = {
    .position = controller->ship->position
  };

  game_event_t event = {
    .type = GAME_EVENT_SHIP_DESTROYED,
    .data = &event_data,
    .data_size = sizeof(ship_destroyed_data_t)
  };

  publish(controller->event_system, &event);

  destroy_ship(controller->ship);
}

void ship_controller_respawn(ship_controller_ptr controller, point_t position,
                             int scale) {
  *controller->ship = create_ship(position, scale);
}
