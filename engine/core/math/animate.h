/**
 * @file animate.h
 * @brief Position animation and screen wrapping utilities
 *
 * Provides functions for animating entity positions based on velocity
 * and delta time. Includes screen-wrapping animation that automatically
 * wraps entities around screen boundaries for continuous movement.
 */

#ifndef ENGINE_CORE_MATH_ANIMATE_H_
#define ENGINE_CORE_MATH_ANIMATE_H_

#include "geometry.h"
#include "graphics.h"
#include "physics.h"

void animate(const point_ptr position, const velocity_ptr velocity,
             double delta_time);

void wrap_animate(const graphics_context_ptr graphics_context,
                  const point_ptr position, const velocity_ptr velocity,
                  double delta_time);

#endif  // ENGINE_CORE_MATH_ANIMATE_H_
