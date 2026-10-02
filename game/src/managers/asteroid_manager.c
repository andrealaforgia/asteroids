#include "asteroid_manager.h"

#include <assert.h>

#include "animate.h"
#include "color.h"
#include "game_constants.h"
#include "game_events.h"
#include "physics.h"
#include "sharpnel.h"
#include "sprites.h"

void init_asteroid_manager(asteroid_manager_ptr manager, game_ptr game,
                           graphics_context_ptr graphics_context,
                           audio_context_ptr audio_context,
                           sharpnel_system_ptr sharpnel_system,
                           event_system_ptr event_system) {
  manager->position_source = random_point;
  manager->game = game;
  manager->graphics_context = graphics_context;
  manager->audio_context = audio_context;
  manager->sharpnel_system = sharpnel_system;
  manager->event_system = event_system;
  manager->pool = create_object_pool(sizeof(asteroid_t), MAX_ASTEROID_COUNT);
}

void destroy_asteroid_manager(asteroid_manager_ptr manager) {
  pool_destroy(&manager->pool);
}

void reset_asteroids(asteroid_manager_ptr manager) {
  pool_reset(&manager->pool);
}

void add_asteroid(asteroid_manager_ptr manager, point_t position, int scale) {
  size_t index;
  asteroid_t* asteroid = (asteroid_t*)pool_acquire(&manager->pool, &index);
  if (asteroid == NULL) {
    return;  // Pool exhausted
  }
  *asteroid = create_asteroid(position, scale, random_color());
}

void remove_asteroid(asteroid_manager_ptr manager, size_t asteroid_index) {
  pool_release(&manager->pool, asteroid_index);
}

// Helper struct to pass both manager and delta_time
typedef struct {
  asteroid_manager_ptr manager;
  double delta_time;
} update_context_t;

static void update_asteroid_with_delta(void* object, size_t index,
                                       void* user_data) {
  (void)index;  // Unused
  update_context_t* ctx = (update_context_t*)user_data;
  asteroid_ptr asteroid = (asteroid_ptr)object;

  wrap_animate(ctx->manager->graphics_context, &asteroid->position,
               &asteroid->velocity, ctx->delta_time);
}

void update_asteroids(asteroid_manager_ptr manager, double delta_time) {
  update_context_t ctx = {manager, delta_time};
  pool_foreach_active(&manager->pool, update_asteroid_with_delta, &ctx);
}

void break_asteroid_apart(asteroid_manager_ptr manager, size_t asteroid_index) {
  asteroid_ptr asteroid =
      (asteroid_ptr)pool_get_at(&manager->pool, asteroid_index);
  if (asteroid == NULL) {
    return;
  }
  add_sharpnel(manager->sharpnel_system, asteroid->position);

  // Publish asteroid destroyed event
  asteroid_destroyed_data_t event_data = {.position = asteroid->position,
                                          .scale = asteroid->scale};

  game_event_t event = {.type = GAME_EVENT_ASTEROID_DESTROYED,
                        .data = &event_data,
                        .data_size = sizeof(asteroid_destroyed_data_t)};

  publish(manager->event_system, &event);

  if (scale_down(asteroid)) {
    // Create two smaller asteroids at the same position
    add_asteroid(manager, asteroid->position, asteroid->scale);
    add_asteroid(manager, asteroid->position, asteroid->scale);
  }
  remove_asteroid(manager, asteroid_index);
}

static point_t farthest_corner(graphics_context_ptr graphics,
                               point_t safe_position) {
  point_t best = point(0, 0);
  for (int x = 0; x < 2; ++x) {
    for (int y = 0; y < 2; ++y) {
      point_t corner = point(x * (graphics->screen_width - 1),
                             y * (graphics->screen_height - 1));
      if (point_distance(&corner, &safe_position) >
          point_distance(&best, &safe_position)) {
        best = corner;
      }
    }
  }
  return best;
}

void create_asteroids(asteroid_manager_ptr manager, point_t safe_position) {
  graphics_context_ptr graphics = manager->graphics_context;
  if (graphics->screen_width <= 0 || graphics->screen_height <= 0) {
    return;
  }
  point_t fallback = farthest_corner(graphics, safe_position);
  double safe_distance = 8 * LARGE_ASTEROID_SCALE * 4;
  double available_distance = point_distance(&fallback, &safe_position);
  if (safe_distance > available_distance * 0.8) {
    safe_distance = available_distance * 0.8;
  }
  size_t count = (size_t)graphics->screen_width * 15 / 1440;
  if (count == 0) {
    count = 1;
  }
  if (count > manager->pool.capacity) {
    count = manager->pool.capacity;
  }
  for (size_t i = 0; i < count; ++i) {
    point_t position = fallback;
    for (int attempt = 0; attempt < 64; ++attempt) {
      point_t candidate = manager->position_source(graphics->screen_width,
                                                   graphics->screen_height);
      if (point_distance(&candidate, &safe_position) > safe_distance) {
        position = candidate;
        break;
      }
    }
    add_asteroid(manager, position, LARGE_ASTEROID_SCALE);
  }
}

void recreate_asteroids_if_none_are_left(asteroid_manager_ptr manager,
                                         point_t safe_position) {
  if (pool_get_active_count(&manager->pool) == 0) {
    create_asteroids(manager, safe_position);
  }
}

size_t get_asteroid_count(const asteroid_manager_ptr manager) {
  return pool_get_active_count(&manager->pool);
}

asteroid_ptr get_asteroid(asteroid_manager_ptr manager, size_t asteroid_index) {
  return (asteroid_ptr)pool_get_at(&manager->pool, asteroid_index);
}

int get_asteroid_radius(const asteroid_manager_ptr manager,
                        size_t asteroid_index) {
  asteroid_ptr asteroid =
      (asteroid_ptr)pool_get_at((object_pool_t*)&manager->pool, asteroid_index);
  if (asteroid == NULL) {
    return 0;
  }
  return 8 * asteroid->scale;
}

void foreach_active_asteroid(asteroid_manager_ptr manager,
                             asteroid_callback_t callback, void* user_data) {
  pool_foreach_active(&manager->pool, (pool_callback_t)callback, user_data);
}

static void render_active_asteroid(void* object, size_t index, void* data) {
  (void)index;
  asteroid_manager_ptr manager = data;
  render_asteroid(manager->graphics_context, object);
}

void render_asteroids(asteroid_manager_ptr manager) {
  pool_foreach_active(&manager->pool, render_active_asteroid, manager);
}
