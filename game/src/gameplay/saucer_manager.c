#include "saucer_manager.h"

#include "animate.h"
#include "clock.h"
#include "game_audio.h"
#include "physics.h"
#include "render.h"
#include "sharpnel.h"

#define SAUCER_CREATION_FREQUENCY_MSECS 30000
#define MAX_SAUCER_BULLET_FIRE_INTERVAL_MSECS 3000

void init_saucer_manager(saucer_manager_ptr manager, game_ptr game,
                         graphics_context_ptr graphics_context,
                         audio_context_ptr audio_context,
                         bullet_manager_ptr bullet_manager) {
  manager->game = game;
  manager->graphics_context = graphics_context;
  manager->audio_context = audio_context;
  manager->bullet_manager = bullet_manager;
  manager->saucer.flying = false;
  manager->last_travel_duration_msecs = 0;
  manager->last_travel_start_ticks = 0;
  manager->last_bullet_fired_ticks = 0;
}

void reset_saucer(saucer_manager_ptr manager) {
  manager->saucer.flying = false;
  manager->last_travel_duration_msecs = 0;
  manager->last_travel_start_ticks = 0;
  manager->last_bullet_fired_ticks = 0;
}

static ALWAYS_INLINE bool sound_on(const saucer_manager_ptr manager) {
  return manager->game->settings.volume > 0;
}

void create_saucer_if_required(saucer_manager_ptr manager) {
  int wait_time = SAUCER_CREATION_FREQUENCY_MSECS +
                  manager->last_travel_duration_msecs;
  if (elapsed_from(manager->last_travel_start_ticks) > wait_time) {
    manager->saucer = create_saucer(manager->graphics_context);
    manager->last_travel_start_ticks = get_clock_ticks_ms();
  }
}

void update_saucer(saucer_manager_ptr manager, double delta_time,
                   point_t target_position) {
  animate(&manager->saucer.position, &manager->saucer.velocity, delta_time);
  if (out_of_bounds(manager->graphics_context, &manager->saucer.position)) {
    manager->saucer.flying = false;
    manager->last_travel_duration_msecs =
        elapsed_from(manager->last_travel_start_ticks);
  } else {
    render_saucer(manager->graphics_context, &manager->saucer);
    if (elapsed_from(manager->last_bullet_fired_ticks) >
        MAX_SAUCER_BULLET_FIRE_INTERVAL_MSECS) {
      add_saucer_bullet(manager->bullet_manager, manager->saucer.position,
                        target_position);
      manager->last_bullet_fired_ticks = get_clock_ticks_ms();
    }
  }
}

void destroy_saucer(saucer_manager_ptr manager) {
  manager->saucer.flying = false;
  add_sharpnel(manager->saucer.position);
  if (is_big(&manager->saucer)) {
    if (sound_on(manager)) {
      play_bang_large(manager->audio_context);
    }
  } else {
    if (sound_on(manager)) {
      play_bang_small(manager->audio_context);
    }
  }
}

bool is_saucer_flying(const saucer_manager_ptr manager) {
  return manager->saucer.flying;
}

int get_saucer_radius(const saucer_manager_ptr manager) {
  return 8 * manager->saucer.scale;
}

point_t get_saucer_position(const saucer_manager_ptr manager) {
  return manager->saucer.position;
}

bool is_saucer_big(const saucer_manager_ptr manager) {
  return is_big(&manager->saucer);
}
