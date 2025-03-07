#ifndef GAME_SETTINGS_H_
#define GAME_SETTINGS_H_

#include <stdbool.h>

#include "window_mode.h"

typedef struct {
  bool no_sound;
  bool show_fps;
  int display;
  int display_mode;
  window_mode_t window_mode;
  int fps;
} game_settings_t;

game_settings_t init_game_settings(bool no_sound, bool show_fps, int display,
                                   int display_mode, window_mode_t window_mode,
                                   int fps);

#endif  // GAME_SETTINGS_H_
