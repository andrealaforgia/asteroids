#include "saucer.h"

#include <stdbool.h>
#include <stdlib.h>

#include "graphics.h"
#include "inline.h"
#include "physics.h"

ALWAYS_INLINE saucer_t _create_saucer(point_t position, velocity_t velocity,
                                      int scale) {
  saucer_t saucer;
  saucer.position = position;
  saucer.velocity = velocity;
  saucer.scale = scale;
  saucer.flying = true;
  return saucer;
}

ALWAYS_INLINE saucer_t
create_saucer(const graphics_context_ptr graphics_context) {
  bool left_to_right = rand() % 100 < 50;
  int scale = rand() % 100 < 50 ? BIG_SAUCER_SCALE : SMALL_SAUCER_SCALE;
  int random_x = left_to_right ? 0 : graphics_context->screen_width - 1;
  int y_offset = graphics_context->screen_height / 10;
  int y_range = graphics_context->screen_height - y_offset;
  int random_y = y_offset + rand() % y_range;
  return _create_saucer(
      point(random_x, random_y),
      left_to_right ? LEFT_TO_RIGHT_VELOCITY : RIGHT_TO_LEFT_VELOCITY, scale);
}

ALWAYS_INLINE bool is_big(const saucer_ptr saucer) {
  return saucer->scale == BIG_SAUCER_SCALE;
}
