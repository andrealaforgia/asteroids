#include "command_line.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HELP "--help"
#define GRAPHICS_INFO "--graphics-info"
#define NO_SOUND "--no-sound"
#define SHOW_FPS "--show-fps"
#define DISPLAY "--display="
#define DISPLAY_MODE "--display-mode="
#define WINDOW_MODE "--window-mode="
#define FPS "--fps="

void print_help(void) {
  puts("\noptions:");
  puts("\t" HELP ": print this help");
  puts("\t" GRAPHICS_INFO ": print info about the graphics system");
  puts("\t" NO_SOUND ": disable sound");
  /* puts("\t" FPS "X : set frame per seconds to X (default is 60)");
  puts("\t" SHOW_FPS ": show frames-per-second stats during game"); */
  puts("\t" DISPLAY "X : use display X (default is 0: use " GRAPHICS_INFO
       " to show the number of available displays");
  puts("\t" DISPLAY_MODE
       "X: use display mode X (default is 0: use " GRAPHICS_INFO
       " to show all available display modes)");
  puts("\t" WINDOW_MODE
       "0|1: run as a maximized window (0 = default) or as full screen (1)");
  printf("\n");
}

static int extract_numeric_argument(const char *prefix, const char *argument,
                                    int *p_number) {
  size_t prefix_length = strlen(prefix);
  size_t argument_length = strlen(argument);
  if (!strncmp(prefix, argument, prefix_length) &&
      argument_length > prefix_length) {
    *p_number = strtoul(&argument[prefix_length], NULL, 0);
    return 1;
  }
  return 0;
}

static void parse_argument(const char *argument,
                           const command_line_options_ptr options) {
  int number;
  if (!strcmp(HELP, argument)) {
    options->help = true;

  } else if (!strcmp(NO_SOUND, argument)) {
    options->no_sound = true;

  } else if (!strcmp(SHOW_FPS, argument)) {
    options->show_fps = true;

  } else if (!strcmp(GRAPHICS_INFO, argument)) {
    options->graphics_info = true;

  } else if (extract_numeric_argument(DISPLAY, argument, &number)) {
    options->display = number;

  } else if (extract_numeric_argument(DISPLAY_MODE, argument, &number)) {
    options->display_mode = number;

  } else if (extract_numeric_argument(WINDOW_MODE, argument, &number)) {
    int valid_window_mode = number == 0 || number == 1;
    if (!valid_window_mode) {
      fprintf(stderr, "Invalid window mode %s\n", argument);
      exit(EXIT_FAILURE);
    }
    options->window_mode = number;

  } else if (extract_numeric_argument(FPS, argument, &number)) {
    options->fps = number;
  }
}

static void set_defaults(const command_line_options_ptr options) {
  options->help = false;
  options->graphics_info = false;
  options->no_sound = false;
  options->show_fps = false;
  options->display = 0;
  options->display_mode = 0;
  options->fps = 60;
}

command_line_options_t parse_command_line_options(int argc, char *argv[]) {
  command_line_options_t options;
  set_defaults(&options);
  for (int i = 0; i < argc; i++) {
    parse_argument(argv[i], &options);
  }
  return options;
}
