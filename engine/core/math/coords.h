#ifndef ENGINE_CORE_MATH_COORDS_H_
#define ENGINE_CORE_MATH_COORDS_H_

typedef struct {
  int lower;
  int upper;
} bounds_t;

typedef struct {
  double x_delta;
  double y_delta;
  int brightness;
} coords_t;

#endif  // ENGINE_CORE_MATH_COORDS_H_
