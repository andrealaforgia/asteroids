#include "sharpnel.h"

#include <assert.h>
#include <stdlib.h>

#include "clock.h"
#include "coords.h"
#include "geometry.h"
#include "graphics.h"
#include "inline.h"
#include "render.h"

#define MAX_SHARPNEL_COUNT 50
#define SHARPNEL_MAX_AGE_MSECS 750

sharpnel_t sharpnels[MAX_SHARPNEL_COUNT];
size_t sharpnel_count = 0;

const bounds_t SHARPNEL_BOUNDS = {49, 69};

ALWAYS_INLINE sharpnel_t create_sharpnel(point_t position) {
  sharpnel_t sharpnel;
  sharpnel.position = position;
  sharpnel.scale = 1;
  sharpnel.creation_ticks = get_clock_ticks_ms();
  return sharpnel;
}

ALWAYS_INLINE void add_sharpnel(point_t position) {
  assert(sharpnel_count < MAX_SHARPNEL_COUNT);
  sharpnels[sharpnel_count++] = create_sharpnel(position);
}

ALWAYS_INLINE void remove_sharpnel(size_t sharpnel_index) {
  assert(sharpnel_count > 0);
  if (sharpnel_count > 1) {
    sharpnels[sharpnel_index] = sharpnels[sharpnel_count - 1];
  }
  --sharpnel_count;
}

ALWAYS_INLINE void render_sharpnel(const graphics_context_ptr graphics_context,
                                   const sharpnel_ptr sharpnel, color_t color) {
  render_object(graphics_context, SHARPNEL_BOUNDS, &sharpnel->position,
                sharpnel->scale, color);
}

ALWAYS_INLINE void update_sharpnel(const graphics_context_ptr graphics_context,
                                   size_t sharpnel_index) {
  int sharpnel_age = elapsed_from(sharpnels[sharpnel_index].creation_ticks);
  if (sharpnel_age > SHARPNEL_MAX_AGE_MSECS) {
    remove_sharpnel(sharpnel_index);
    return;
  } else {
    sharpnels[sharpnel_index].scale += 0.375;
  }
  color_t color = GRAY_SCALE(sharpnel_age, SHARPNEL_MAX_AGE_MSECS);
  render_sharpnel(graphics_context, &sharpnels[sharpnel_index], color);
}

ALWAYS_INLINE void animate_sharpnels(
    const graphics_context_ptr graphics_context) {
  for (size_t sbi = 0; sbi < sharpnel_count; sbi++) {
    update_sharpnel(graphics_context, sbi);
  }
}

void reset_sharpnels(void) { sharpnel_count = 0; }
