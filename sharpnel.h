#ifndef SHARPNEL_H_
#define SHARPNEL_H_

#include <stdbool.h>

#include "geometry.h"

typedef struct {
  point_t position;
  double scale;
  int creation_ticks;
  bool active;
} sharpnel_t, *sharpnel_ptr;

sharpnel_t create_sharpnel(point_t position);

#endif  // SHARPNEL_H_
