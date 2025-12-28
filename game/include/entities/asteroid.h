#ifndef ASTEROID_H_
#define ASTEROID_H_

#include <stdbool.h>

#include "color.h"
#include "geometry.h"
#include "physics.h"

#define LARGE_ASTEROID_SCALE 6
#define MEDIUM_ASTEROID_SCALE 3
#define SMALL_ASTEROID_SCALE 2

typedef struct {
  int type;
  int scale;
  point_t position;
  velocity_t velocity;
  color_t color;
} asteroid_t, *asteroid_ptr;

asteroid_t create_asteroid(point_t position, int scale, color_t color);
bool scale_down(const asteroid_ptr asteroid);
int random_asteroid_scale(void);

#endif  // ASTEROID_H_
