#include "ship_controller.h"

#include "animate.h"
#include "clock.h"
#include "game_audio.h"
#include "game_constants.h"
#include "game_events.h"
#include "inline.h"
#include "physics.h"
#include "sprites.h"

ship_controller_t create_ship_controller(ship_ptr ship,
                                         graphics_context_ptr graphics,
                                         audio_context_ptr audio,
                                         bullet_manager_ptr bullets,
                                         sharpnel_system_ptr sharpnel,
                                         event_system_ptr event_system) {
  ship_controller_t controller = {0};
  controller.ship = ship;
  controller.graphics_context = graphics;
  controller.audio_context = audio;
  controller.bullet_manager = bullets;
  controller.sharpnel_system = sharpnel;
  controller.event_system = event_system;
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
}

void render_ship_controller(ship_controller_ptr controller) {
  if (elapsed_from(controller->ship->creation_ticks) >
          SHIP_IMMUNITY_DURATION_MS ||
      (get_clock_ticks_ms() / 100) % 2 == 0) {
    render_ship(controller->graphics_context, controller->ship);
  }
}

void ship_controller_handle_thrust(ship_controller_ptr controller,
                                   double delta_time) {
  accelerate_ship(controller->ship, delta_time);
  if (controller->thrust_sound_remaining <= 0) {
    play_thrust(controller->audio_context);
    controller->thrust_sound_remaining = SHIP_THRUST_INTERVAL_MS;
  }
}

void ship_controller_handle_fire(ship_controller_ptr controller) {
  point_t bullet_position = get_cannon_position(controller->ship);
  velocity_t bullet_velocity =
      velocity(BULLET_SPEED, controller->ship->rotation_vector);
  add_ship_bullet(controller->bullet_manager, bullet_position, bullet_velocity);
  play_fire(controller->audio_context);
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
  if (controller->ship->state == DESTROYED) {
    return;
  }
  destroy_ship(controller->ship);
  add_sharpnel(controller->sharpnel_system, controller->ship->position);

  // Publish ship destroyed event
  ship_destroyed_data_t event_data = {.position = controller->ship->position};

  game_event_t event = {.type = GAME_EVENT_SHIP_DESTROYED,
                        .data = &event_data,
                        .data_size = sizeof(ship_destroyed_data_t)};

  publish(controller->event_system, &event);
}

void ship_controller_respawn(ship_controller_ptr controller, point_t position,
                             int scale) {
  *controller->ship = create_ship(position, scale);
  controller->rotation_remainder = 0;
  controller->fire_remaining = 0;
  controller->thrust_sound_remaining = 0;
}

void ship_controller_apply_input(ship_controller_ptr controller,
                                 ship_input_t input, double delta_time) {
  double milliseconds = delta_time * 1000.0 / PHYSICS_BASELINE_FPS;
  controller->fire_remaining -= milliseconds;
  controller->thrust_sound_remaining -= milliseconds;
  if (input.left != input.right) {
    controller->rotation_remainder += milliseconds / SHIP_ROTATION_INTERVAL_MS;
    while (controller->rotation_remainder >= 1.0) {
      if (input.left) {
        ship_controller_handle_rotate_left(controller);
      } else {
        ship_controller_handle_rotate_right(controller);
      }
      controller->rotation_remainder -= 1.0;
    }
  } else {
    controller->rotation_remainder = 0;
  }
  if (input.thrust) {
    ship_controller_handle_thrust(controller, delta_time);
  }
  if (input.fire && controller->fire_remaining <= 0) {
    ship_controller_handle_fire(controller);
    controller->fire_remaining = SHIP_FIRE_INTERVAL_MS;
  }
}
