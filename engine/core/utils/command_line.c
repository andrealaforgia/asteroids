#include "command_line.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HELP "--help"
#define GRAPHICS_INFO "--graphics-info"
#define SHOW_FPS "--show-fps"
#define VSYNC "--vsync"
#define DISPLAY "--display="
#define DISPLAY_MODE "--display-mode="
#define WINDOW_MODE "--window-mode="
#define FPS "--fps="
#define VOLUME "--volume="

void print_help(void) {
  puts("\noptions:");
  puts("\t" HELP ": print this help");
  puts("\t" GRAPHICS_INFO ": print info about the graphics system");
  puts("\t" VSYNC ": enable VSync for smoother rendering (default: off)");
  puts("\t" VOLUME
       "X: set audio volume 0-128 (default: 32, 0=silent, 128=max)");
  puts("\t" SHOW_FPS ": show frames-per-second stats during game");
  puts("\t" FPS "X: set target frame rate 1-300 (default: 60)");
  puts("\t" DISPLAY "X : use display X (default is 0: use " GRAPHICS_INFO
       " to show the number of available displays");
  puts("\t" DISPLAY_MODE
       "X: use display mode X (default is 0: use " GRAPHICS_INFO
       " to show all available display modes)");
  puts("\t" WINDOW_MODE
       "N: window mode (0=windowed, 1=fullscreen, 2=borderless, 3=maximized)");
  printf("\n");
}

static int extract_numeric_argument(const char* prefix, const char* argument,
                                    int* p_number) {
  size_t prefix_length = strlen(prefix);
  size_t argument_length = strlen(argument);
  if (!strncmp(prefix, argument, prefix_length) &&
      argument_length > prefix_length) {
    *p_number = strtoul(&argument[prefix_length], NULL, 0);
    return 1;
  }
  return 0;
}

// Returns true if the argument was recognized, false otherwise
static bool parse_argument(const char* argument,
                           const command_line_options_ptr options) {
  int number;
  if (!strcmp(HELP, argument)) {
    options->help = true;

  } else if (!strcmp(SHOW_FPS, argument)) {
    options->show_fps = true;

  } else if (!strcmp(VSYNC, argument)) {
    options->vsync = true;

  } else if (!strcmp(GRAPHICS_INFO, argument)) {
    options->graphics_info = true;

  } else if (extract_numeric_argument(DISPLAY, argument, &number)) {
    options->display = number;

  } else if (extract_numeric_argument(DISPLAY_MODE, argument, &number)) {
    options->display_mode = number;

  } else if (extract_numeric_argument(WINDOW_MODE, argument, &number)) {
    int valid_window_mode = number >= 0 && number <= 3;
    if (!valid_window_mode) {
      fprintf(stderr, "Error: Invalid window mode %s (valid: 0-3)\n", argument);
      fprintf(stderr, "Use --help to see all available options\n");
      exit(EXIT_FAILURE);
    }
    options->window_mode = number;

  } else if (extract_numeric_argument(FPS, argument, &number)) {
    if (number < 1 || number > 300) {
      fprintf(stderr, "Error: Invalid FPS %s (valid: 1-300)\n", argument);
      fprintf(stderr, "Use --help to see all available options\n");
      exit(EXIT_FAILURE);
    }
    options->fps = number;

  } else if (extract_numeric_argument(VOLUME, argument, &number)) {
    if (number < 0 || number > 128) {
      fprintf(stderr, "Error: Invalid volume %s (valid: 0-128)\n", argument);
      fprintf(stderr, "Use --help to see all available options\n");
      exit(EXIT_FAILURE);
    }
    options->volume = number;

  } else {
    // Argument not recognized
    return false;
  }

  return true;
}

static void set_defaults(const command_line_options_ptr options) {
  options->help = false;
  options->graphics_info = false;
  options->show_fps = false;
  options->vsync = false;
  options->display = 0;
  options->display_mode = 0;
  options->window_mode = FULLSCREEN;  // Default to fullscreen
  options->fps = 60;
  options->volume = 32;  // Default volume (0-128 range)
}

command_line_options_t parse_command_line_options(int argc, char* argv[]) {
  command_line_options_t options;
  set_defaults(&options);

  // Skip argv[0] which is the program name
  for (int i = 1; i < argc; i++) {
    if (!parse_argument(argv[i], &options)) {
      // Unrecognized argument
      if (strncmp(argv[i], "--", 2) == 0 || strncmp(argv[i], "-", 1) == 0) {
        fprintf(stderr, "Error: Unknown option '%s'\n", argv[i]);
        fprintf(stderr, "Use --help to see all available options\n");
        exit(EXIT_FAILURE);
      }
      // Not an option (doesn't start with --), ignore it
    }
  }

  return options;
}
