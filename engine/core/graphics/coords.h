/**
 * @file coords.h
 * @brief Coordinate and boundary type definitions
 *
 * Defines types for coordinate transformations and boundary ranges.
 * Used for particle effects and other graphical calculations requiring
 * coordinate deltas and brightness values.
 */

#ifndef ENGINE_CORE_GRAPHICS_COORDS_H_
#define ENGINE_CORE_GRAPHICS_COORDS_H_

typedef struct {
  int lower;
  int upper;
} bounds_t;

typedef struct {
  double x_delta;
  double y_delta;
  int brightness;
} coords_t;

#endif  // ENGINE_CORE_GRAPHICS_COORDS_H_
