/**
 * @file frame_limiter.h
 * @brief Frame rate limiting and delta time calculation
 *
 * Manages frame timing to maintain a target FPS and provides normalized
 * delta time for physics calculations. Yields CPU time between frames
 * and ensures consistent game speed across different refresh rates.
 */

#ifndef ENGINE_CORE_GRAPHICS_FRAME_LIMITER_H_
#define ENGINE_CORE_GRAPHICS_FRAME_LIMITER_H_

#include "clock.h"

// Frame rate limiter for consistent timing
typedef struct {
  int target_fps;
  int last_frame_ticks;
  double fps_baseline;  // For delta_time normalization (60 FPS)
} frame_limiter_t;

// Create a frame limiter with specified target FPS
frame_limiter_t create_frame_limiter(int target_fps);

// Wait for next frame and return normalized delta_time
// Returns delta_time normalized to 60 FPS baseline
// Yields CPU with SDL_Delay(1) when waiting
double frame_limiter_wait(frame_limiter_t* limiter);

#endif  // ENGINE_CORE_GRAPHICS_FRAME_LIMITER_H_
