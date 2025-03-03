#ifndef PHYSICS_H_
#define PHYSICS_H_

#include "geometry.h"

typedef struct {
  double speed;
  vector_t direction;
} velocity_t, *velocity_ptr;

velocity_t velocity(const double speed, const vector_t direction);
velocity_t random_velocity(void);

#endif  // PHYSICS_H_
