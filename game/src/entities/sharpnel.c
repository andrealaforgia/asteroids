#include "sharpnel.h"

#include <assert.h>
#include <stdlib.h>

#include "clock.h"
#include "coords.h"
#include "game_constants.h"
#include "geometry.h"
#include "graphics.h"
#include "inline.h"
#include "sprites.h"

#define SHARPNEL_MAX_AGE_MSECS 750

const bounds_t SHARPNEL_BOUNDS = {49, 69};

sharpnel_system_t* create_sharpnel_system(
    graphics_context_ptr graphics_context, size_t max_count) {
  sharpnel_system_t* system = malloc(sizeof(sharpnel_system_t));
  system->graphics_context = graphics_context;
  system->pool = create_object_pool(sizeof(sharpnel_t), max_count);
  return system;
}

void destroy_sharpnel_system(sharpnel_system_t* system) {
  pool_destroy(&system->pool);
  free(system);
}

void reset_sharpnels(sharpnel_system_t* system) {
  pool_reset(&system->pool);
}

void add_sharpnel(sharpnel_system_t* system, point_t position) {
  size_t index;
  sharpnel_t* sharpnel = (sharpnel_t*)pool_acquire(&system->pool, &index);
  if (sharpnel == NULL) {
    return;  // Pool exhausted
  }

  sharpnel->position = position;
  sharpnel->scale = 1;
  sharpnel->creation_ticks = get_clock_ticks_ms();
}

void animate_sharpnels(sharpnel_system_t* system, double delta_time) {
  // Iterate backwards through pool capacity to safely handle removals
  for (int sbi = system->pool.capacity - 1; sbi >= 0; sbi--) {
    if (!pool_is_active(&system->pool, sbi)) {
      continue;
    }

    sharpnel_t* sharpnel = (sharpnel_t*)pool_get_at(&system->pool, sbi);
    if (sharpnel == NULL) {
      continue;  // Safety check
    }

    int sharpnel_age = elapsed_from(sharpnel->creation_ticks);

    if (sharpnel_age > SHARPNEL_MAX_AGE_MSECS) {
      pool_release(&system->pool, sbi);
      continue;
    }

    sharpnel->scale += 0.375 * delta_time;
    color_t color = GRAY_SCALE(sharpnel_age, SHARPNEL_MAX_AGE_MSECS);
    render_object(system->graphics_context, SHARPNEL_BOUNDS,
                  &sharpnel->position, sharpnel->scale, color);
  }
}
