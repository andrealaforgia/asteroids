#include "fps_tracker.h"

#include <stdio.h>

#include "clock.h"
#include "inline.h"

ALWAYS_INLINE fps_tracker_t create_fps_tracker(void) {
  fps_tracker_t fps_tracker;
  fps_tracker.frame_count = 0;
  fps_tracker.start_ticks = get_clock_ticks_ms();
  return fps_tracker;
}

ALWAYS_INLINE void track_fps(const fps_tracker_ptr fps_tracker) {
  fps_tracker->frame_count++;
}

ALWAYS_INLINE void format_fps(const fps_tracker_ptr fps_tracker, char* s,
                              size_t n) {
  snprintf(s, n, "FPS %u",
           (unsigned)(1000.0 * fps_tracker->frame_count) /
               elapsed_from(fps_tracker->start_ticks));
}
