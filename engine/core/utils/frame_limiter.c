#include "frame_limiter.h"

#include <stdbool.h>

#include <SDL.h>

frame_limiter_t create_frame_limiter(int target_fps) {
  frame_limiter_t limiter;
  limiter.target_fps = target_fps;
  limiter.last_frame_ticks = get_clock_ticks_ms();
  limiter.fps_baseline = 60.0;
  return limiter;
}

double frame_limiter_wait(frame_limiter_t* limiter) {
  // Calculate target frame time in milliseconds
  int frame_time = 1000 / limiter->target_fps;

  // Wait until enough time has elapsed for the next frame
  int elapsed;
  while (true) {
    elapsed = elapsed_from(limiter->last_frame_ticks);
    if (elapsed >= frame_time) {
      break;
    }
    // Yield CPU to avoid busy waiting
    SDL_Delay(1);
  }

  // Update last frame time
  limiter->last_frame_ticks = get_clock_ticks_ms();

  // Calculate delta_time normalized to 60 FPS baseline
  // This ensures physics calculations remain consistent across different FPS
  double delta_time = elapsed / (1000.0 / limiter->fps_baseline);

  return delta_time;
}
