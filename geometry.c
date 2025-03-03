#include "geometry.h"

#include <math.h>
#include <stdlib.h>

#include "inline.h"

ALWAYS_INLINE point_t point(double x, double y) {
  point_t p;
  p.x = x;
  p.y = y;
  return p;
}

ALWAYS_INLINE vector_t vector(double x, double y) {
  vector_t v;
  v.x = x;
  v.y = y;
  return v;
}

ALWAYS_INLINE vector_t points_vector(const point_ptr a, const point_ptr b) {
  double angle = atan2(b->y - a->y, b->x - a->x);
  return vector(cos(angle), sin(angle));
}

ALWAYS_INLINE point_t relative_point(const point_ptr p,
                                     const rel_point_ptr rel_p) {
  return point(p->x + rel_p->x_delta, p->y + rel_p->y_delta);
}

ALWAYS_INLINE double point_distance(const point_ptr p1, const point_ptr p2) {
  int distX = p1->x - p2->x;
  int distY = p1->y - p2->y;
  return sqrt(distY * distY + distX * distX);
}

ALWAYS_INLINE double random_angle(void) {
  return (rand() % 101) / 100.0 * 2 * M_PI;
}

ALWAYS_INLINE point_t random_point_around(const point_ptr p, int min_tolerance,
                                          int max_tolerance) {
  int rndX =
      rand() % max_tolerance + min_tolerance * (rand() % 10 < 5 ? 1 : -1);
  int rndY =
      rand() % max_tolerance + min_tolerance * (rand() % 10 < 5 ? 1 : -1);
  return point(p->x + rndX, p->y + rndY);
}
