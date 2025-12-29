#include "game_settings.h"

#include <stdbool.h>

#include "window_mode.h"

game_settings_t init_game_settings(bool show_fps, bool vsync, int display,
                                   int display_mode, window_mode_t window_mode,
                                   int fps, int volume, int initial_lives) {
  game_settings_t game_settings;
  game_settings.show_fps = show_fps;
  game_settings.vsync = vsync;
  game_settings.display = display;
  game_settings.display_mode = display_mode;
  game_settings.window_mode = window_mode;
  game_settings.fps = fps;
  game_settings.volume = volume;
  game_settings.initial_lives = initial_lives;
  return game_settings;
}
