#include "animate.h"

#include "geometry.h"
#include "graphics.h"
#include "inline.h"
#include "physics.h"

ALWAYS_INLINE void animate(const point_ptr position,
                           const velocity_ptr velocity, double delta_time) {
  position->x += velocity->speed * velocity->direction.x * delta_time;
  position->y += velocity->speed * velocity->direction.y * delta_time;
}

ALWAYS_INLINE void wrap_animate(const graphics_context_ptr graphics_context,
                                const point_ptr position,
                                const velocity_ptr velocity,
                                double delta_time) {
  animate(position, velocity, delta_time);
  position->x = wrap_x(graphics_context, position->x);
  position->y = wrap_y(graphics_context, position->y);
}
