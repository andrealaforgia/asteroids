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

static asteroid_t asteroids[MAX_ASTEROID_COUNT];
static size_t asteroid_count = 0;

static asteroid_manager_t* manager_instance = NULL;

void init_asteroid_manager(asteroid_manager_ptr manager, game_ptr game,
                            graphics_context_ptr graphics_context,
                            audio_context_ptr audio_context) {
  manager->game = game;
  manager->graphics_context = graphics_context;
  manager->audio_context = audio_context;
  manager_instance = manager;
  asteroid_count = 0;
}

void reset_asteroids(asteroid_manager_ptr manager) {
  (void)manager;  // Unused parameter
  asteroid_count = 0;
}

void add_asteroid(asteroid_manager_ptr manager, point_t position, int scale) {
  (void)manager;  // Unused parameter
  assert(asteroid_count < MAX_ASTEROID_COUNT);
  asteroids[asteroid_count++] =
      create_asteroid(position, scale, random_color());
}

void remove_asteroid(asteroid_manager_ptr manager, size_t asteroid_index) {
  (void)manager;  // Unused parameter
  assert(asteroid_count > 0);
  if (asteroid_count > 1) {
    asteroids[asteroid_index] = asteroids[asteroid_count - 1];
  }
  --asteroid_count;
}

static ALWAYS_INLINE void animate_asteroid(
    graphics_context_ptr graphics_context, size_t asteroid_index,
    double delta_time) {
  asteroid_ptr asteroid = &asteroids[asteroid_index];
  wrap_animate(graphics_context, &asteroid->position, &asteroid->velocity,
               delta_time);
}

static ALWAYS_INLINE void update_asteroid(
    graphics_context_ptr graphics_context, size_t asteroid_index,
    double delta_time) {
  animate_asteroid(graphics_context, asteroid_index, delta_time);
  render_asteroid(graphics_context, &asteroids[asteroid_index]);
}

void update_asteroids(asteroid_manager_ptr manager, double delta_time) {
  for (size_t ai = 0; ai < asteroid_count; ai++) {
    update_asteroid(manager->graphics_context, ai, delta_time);
  }
}

static ALWAYS_INLINE bool sound_on(const asteroid_manager_ptr manager) {
  return manager->game->settings.volume > 0;
}

void break_asteroid_apart(asteroid_manager_ptr manager,
                          size_t asteroid_index) {
  asteroid_ptr asteroid = &asteroids[asteroid_index];
  add_sharpnel(asteroid->position);

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
    add_asteroid(manager, asteroid->position, asteroid->scale);
  } else {
    remove_asteroid(manager, asteroid_index);
  }
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
  if (asteroid_count == 0) {
    create_asteroids(manager, safe_position);
  }
}

size_t get_asteroid_count(const asteroid_manager_ptr manager) {
  (void)manager;  // Unused parameter
  return asteroid_count;
}

asteroid_ptr get_asteroid(asteroid_manager_ptr manager, size_t asteroid_index) {
  (void)manager;  // Unused parameter
  assert(asteroid_index < asteroid_count);
  return &asteroids[asteroid_index];
}

int get_asteroid_radius(const asteroid_manager_ptr manager,
                        size_t asteroid_index) {
  (void)manager;  // Unused parameter
  assert(asteroid_index < asteroid_count);
  return 8 * asteroids[asteroid_index].scale;
}
