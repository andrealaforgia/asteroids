#ifndef ENGINE_CORE_MATH_ANIMATE_H_
#define ENGINE_CORE_MATH_ANIMATE_H_

#include "geometry.h"
#include "graphics.h"
#include "physics.h"

void animate(const point_ptr position, const velocity_ptr velocity);

void wrap_animate(const graphics_context_ptr graphics_context,
                  const point_ptr position, const velocity_ptr velocity);

#endif  // ENGINE_CORE_MATH_ANIMATE_H_
