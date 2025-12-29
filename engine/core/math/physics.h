/**
 * @file physics.h
 * @brief Basic physics types and velocity calculations
 *
 * Defines velocity as a combination of speed and direction vector.
 * Provides functions for creating velocities and generating random
 * velocity vectors for entity movement.
 */

#ifndef ENGINE_CORE_MATH_PHYSICS_H_
#define ENGINE_CORE_MATH_PHYSICS_H_

#include "geometry.h"

typedef struct {
  double speed;
  vector_t direction;
} velocity_t, *velocity_ptr;

velocity_t velocity(const double speed, const vector_t direction);
velocity_t random_velocity(void);

#endif  // ENGINE_CORE_MATH_PHYSICS_H_
