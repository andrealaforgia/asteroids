#include "game_settings.h"

#include <stdbool.h>

#include "window_mode.h"

game_settings_t init_game_settings(bool no_sound, bool show_fps, int display,
                                   int display_mode, window_mode_t window_mode,
                                   int fps) {
  game_settings_t game_settings;
  game_settings.no_sound = no_sound;
  game_settings.show_fps = show_fps;
  game_settings.display = display;
  game_settings.display_mode = display_mode;
  game_settings.window_mode = window_mode;
  game_settings.fps = fps;
  return game_settings;
}
