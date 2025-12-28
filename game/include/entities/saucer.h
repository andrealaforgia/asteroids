#ifndef SAUCER_H_
#define SAUCER_H_

#include <stdbool.h>

#include "geometry.h"
#include "graphics.h"
#include "physics.h"

#define SMALL_SAUCER_SCALE 2
#define BIG_SAUCER_SCALE 3

#define LEFT_TO_RIGHT_VELOCITY (velocity(4, vector(1, 0)))
#define RIGHT_TO_LEFT_VELOCITY (velocity(4, vector(-1, 0)))

typedef struct {
  point_t position;
  velocity_t velocity;
  int scale;
  bool flying;
} saucer_t, *saucer_ptr;

saucer_t create_saucer(const graphics_context_ptr graphics_context);
bool is_big(const saucer_ptr saucer);

#endif  // SAUCER_H_
