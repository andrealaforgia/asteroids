#ifndef COMMAND_LINE_H_
#define COMMAND_LINE_H_

#include <stdbool.h>

#include "window_mode.h"

typedef struct {
  bool help;
  bool graphics_info;
  bool no_sound;
  bool show_fps;
  bool vsync;
  int display;
  int display_mode;
  window_mode_t window_mode;
  int fps;
  int volume;  // Audio volume (0-128, default 32)
} command_line_options_t, *command_line_options_ptr;

command_line_options_t parse_command_line_options(int argc, char *argv[]);
void print_help(void);

#endif  // COMMAND_LINE_H_
