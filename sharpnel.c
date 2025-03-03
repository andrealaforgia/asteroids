#include "sharpnel.h"

#include <stdlib.h>

#include "clock.h"
#include "geometry.h"
#include "inline.h"

ALWAYS_INLINE sharpnel_t create_sharpnel(point_t position) {
  sharpnel_t sharpnel;
  sharpnel.position = position;
  sharpnel.scale = 1;
  sharpnel.creation_ticks = get_clock_ticks_ms();
  return sharpnel;
}
