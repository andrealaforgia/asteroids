/**
 * @file game_settings.h
 * @brief Game configuration settings
 *
 * Defines the game settings structure containing display options, FPS limits,
 * audio volume, and gameplay parameters like initial lives. Initialized from
 * command-line arguments and used throughout the game.
 */

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
