#ifndef FPS_TRACKER_H_
#define FPS_TRACKER_H_

#include <stdlib.h>

typedef struct {
  unsigned frame_count;
  int start_ticks;
} fps_tracker_t, *fps_tracker_ptr;

fps_tracker_t create_fps_tracker(void);
void track_fps(const fps_tracker_ptr fps_tracker);
void format_fps(const fps_tracker_ptr fps_tracker, char *s, size_t n);

#endif  // FPS_TRACKER_H_
