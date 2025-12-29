/**
 * @file command_line.h
 * @brief Command-line argument parsing for game configuration
 *
 * Parses command-line arguments to configure display settings, window mode,
 * FPS limits, audio volume, and other runtime options. Provides help text
 * and validation for all supported command-line flags.
 */

#ifndef ENGINE_CORE_UTILS_COMMAND_LINE_H_
#define ENGINE_CORE_UTILS_COMMAND_LINE_H_

#include <stdbool.h>

#include "window_mode.h"

typedef struct {
  bool help;
  bool graphics_info;
  bool show_fps;
  bool vsync;
  int display;
  int display_mode;
  window_mode_t window_mode;
  int fps;
  int volume;  // Audio volume (0-128, default 32)
} command_line_options_t, *command_line_options_ptr;

command_line_options_t parse_command_line_options(int argc, char* argv[]);
void print_help(void);

#endif  // ENGINE_CORE_UTILS_COMMAND_LINE_H_
