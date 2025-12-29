#include "asteroid_manager.h"

#include <assert.h>

#include "animate.h"
#include "color.h"
#include "game_audio.h"
#include "game_constants.h"
#include "physics.h"
#include "render.h"
#include "score.h"
#include "sharpnel.h"

void init_asteroid_manager(asteroid_manager_ptr manager, game_ptr game,
                            graphics_context_ptr graphics_context,
                            audio_context_ptr audio_context,
                            sharpnel_system_ptr sharpnel_system) {
  manager->game = game;
  manager->graphics_context = graphics_context;
  manager->audio_context = audio_context;
  manager->sharpnel_system = sharpnel_system;
  manager->pool = create_object_pool(sizeof(asteroid_t), MAX_ASTEROID_COUNT);
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
  render_asteroid(ctx->manager->graphics_context, asteroid);
}

void update_asteroids(asteroid_manager_ptr manager, double delta_time) {
  update_context_t ctx = {manager, delta_time};
  pool_foreach_active(&manager->pool, update_asteroid_with_delta, &ctx);
}

static ALWAYS_INLINE bool sound_on(const asteroid_manager_ptr manager) {
  return manager->game->settings.volume > 0;
}

void break_asteroid_apart(asteroid_manager_ptr manager,
                          size_t asteroid_index) {
  asteroid_ptr asteroid = (asteroid_ptr)pool_get_at(&manager->pool, asteroid_index);
  if (asteroid == NULL) {
    return;
  }
  add_sharpnel(manager->sharpnel_system, asteroid->position);

  switch (asteroid->scale) {
    case LARGE_ASTEROID_SCALE: {
      if (sound_on(manager)) {
        play_bang_large(manager->audio_context);
      }
      score_large_asteroid(manager->game);
      break;
    }
    case MEDIUM_ASTEROID_SCALE: {
      if (sound_on(manager)) {
        play_bang_medium(manager->audio_context);
      }
      score_medium_asteroid(manager->game);
      break;
    }
    case SMALL_ASTEROID_SCALE: {
      if (sound_on(manager)) {
        play_bang_small(manager->audio_context);
      }
      score_small_asteroid(manager->game);
      break;
    }
  }

  if (scale_down(asteroid)) {
    // Create two smaller asteroids at the same position
    add_asteroid(manager, asteroid->position, asteroid->scale);
    add_asteroid(manager, asteroid->position, asteroid->scale);
  }
  remove_asteroid(manager, asteroid_index);
}

void create_asteroids(asteroid_manager_ptr manager, point_t safe_position) {
  int distance_from_ship = 8 * LARGE_ASTEROID_SCALE * 4;
  size_t initial_asteroid_count =
      (manager->graphics_context->screen_width * 15) / 1440;
  for (size_t i = 0; i < initial_asteroid_count; i++) {
    while (true) {
      point_t asteroid_position = random_point(manager->graphics_context);
      if (point_distance(&asteroid_position, &safe_position) >
          distance_from_ship) {
        add_asteroid(manager, asteroid_position, LARGE_ASTEROID_SCALE);
        break;
      }
    }
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
  asteroid_ptr asteroid = (asteroid_ptr)pool_get_at((object_pool_t*)&manager->pool, asteroid_index);
  if (asteroid == NULL) {
    return 0;
  }
  return 8 * asteroid->scale;
}
