#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "command_line.h"
#include "game.h"
#include "graphics.h"
#include "logger.h"
#include "stage.h"

static bool run_game(const game_ptr game) {
  // Create stage instances
  stage_ptr stages[3] = {create_intro_stage_instance(),
                         create_playing_stage_instance(),
                         create_game_over_stage_instance()};

  for (int i = 0; i < 3; ++i) {
    if (!stages[i]) {
      for (int j = 0; j < 3; ++j) {
        destroy_stage(stages[j]);
      }
      LOG_ERROR("Unable to allocate game stages");
      return false;
    }
  }

  // Start with intro stage
  int current_stage_index = 0;
  stage_ptr current_stage = stages[current_stage_index];
  current_stage->init(current_stage, game);

  while (current_stage->state != NULL) {
    game_stage_action_t action = current_stage->update(current_stage);

    if (action == QUIT) {
      break;
    }

    if (action == PROGRESS) {
      current_stage->cleanup(current_stage);

      // Determine next stage
      if (current_stage_index == 0) {
        // INTRO -> PLAYING
        current_stage_index = 1;
      } else if (current_stage_index == 1) {
        // PLAYING -> GAME_OVER
        current_stage_index = 2;
      } else {
        // GAME_OVER -> PLAYING
        current_stage_index = 1;
        reset_game(game);
      }

      current_stage = stages[current_stage_index];
      current_stage->init(current_stage, game);
    }
  }

  bool succeeded = current_stage->state != NULL;
  if (!succeeded) {
    LOG_ERROR("Unable to allocate game stage state");
  }
  // Cleanup all stages
  for (int i = 0; i < 3; i++) {
    destroy_stage(stages[i]);
  }
  return succeeded;
}

int main(int argc, char* argv[]) {
  srand(time(NULL));

  command_line_options_t command_line_options =
      parse_command_line_options(argc, argv);

  if (command_line_options.help) {
    print_help();
    return 0;
  }

  if (command_line_options.graphics_info) {
    print_graphics_info();
    return 0;
  }

  game_settings_t game_settings = init_game_settings(
      command_line_options.show_fps, command_line_options.vsync,
      command_line_options.display, command_line_options.display_mode,
      command_line_options.window_mode, command_line_options.fps,
      command_line_options.volume, 5);

  // Log game configuration
  LOG_INFO("=== Asteroids Configuration ===");
  LOG_INFO_FMT("Display: %d, Mode: %d", game_settings.display,
               game_settings.display_mode);
  const char* window_modes[] = {"Windowed", "Fullscreen", "Borderless",
                                "Maximized"};
  LOG_INFO_FMT("Window Mode: %s", window_modes[game_settings.window_mode]);
  LOG_INFO_FMT("VSync: %s", game_settings.vsync ? "Enabled" : "Disabled");
  LOG_INFO_FMT("Target FPS: %d", game_settings.fps);
  LOG_INFO_FMT("Show FPS: %s", game_settings.show_fps ? "Yes" : "No");
  LOG_INFO_FMT("Audio Volume: %d/128", game_audio_volume(&game_settings));
  LOG_INFO("==============================");

  game_t game = init_game(game_settings);

  bool succeeded = run_game(&game);

  terminate_game(&game);

  return succeeded ? EXIT_SUCCESS : EXIT_FAILURE;
}
