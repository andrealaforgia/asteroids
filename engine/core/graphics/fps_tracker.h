/**
 * @file fps_tracker.h
 * @brief Frame rate measurement and tracking
 *
 * Provides a simple FPS tracker that counts frames and calculates
 * average frames per second. Used for performance monitoring and
 * displaying FPS information on screen.
 */

#ifndef ENGINE_CORE_GRAPHICS_FPS_TRACKER_H_
#define ENGINE_CORE_GRAPHICS_FPS_TRACKER_H_

#include <stdlib.h>

typedef struct {
  unsigned frame_count;
  int start_ticks;
} fps_tracker_t, *fps_tracker_ptr;

fps_tracker_t create_fps_tracker(void);
void track_fps(const fps_tracker_ptr fps_tracker);
void format_fps(const fps_tracker_ptr fps_tracker, char* s, size_t n);

#endif  // ENGINE_CORE_GRAPHICS_FPS_TRACKER_H_
