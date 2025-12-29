#ifndef GAME_SRC_MAIN_GAME_SETTINGS_H_
#define GAME_SRC_MAIN_GAME_SETTINGS_H_

#include <stdbool.h>

#include "window_mode.h"

typedef struct {
  bool show_fps;
  bool vsync;
  int display;
  int display_mode;
  window_mode_t window_mode;
  int fps;
  int volume;
  int initial_lives;
} game_settings_t;

game_settings_t init_game_settings(bool show_fps, bool vsync, int display,
                                   int display_mode, window_mode_t window_mode,
                                   int fps, int volume, int initial_lives);

#endif  // GAME_SRC_MAIN_GAME_SETTINGS_H_
