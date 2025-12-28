#ifndef COORDS_H_
#define COORDS_H_

typedef struct {
  int lower;
  int upper;
} bounds_t;

typedef struct {
  double x_delta;
  double y_delta;
  int brightness;
} coords_t;

#endif  // COORDS_H_
