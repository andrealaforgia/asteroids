#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "command_line.h"
#include "game.h"
#include "game_over_stage.h"
#include "graphics.h"
#include "intro_stage.h"
#include "logger.h"
#include "playing_stage.h"

static void run_game(const game_ptr game) {
  game_stage_t game_stage = INTRO;

  intro_stage_state_ptr intro_stage = create_intro_stage(game);
  playing_stage_state_ptr playing_stage = create_playing_stage(game);
  game_over_stage_state_ptr game_over_stage = create_game_over_stage(game);

  while (true) {
    switch (game_stage) {
      case INTRO: {
        game_stage_action_t action = handle_intro_stage(intro_stage);
        switch (action) {
          case PROGRESS:
            game_stage = PLAYING;
            break;
          case QUIT:
            destroy_intro_stage(intro_stage);
            destroy_playing_stage(playing_stage);
            destroy_game_over_stage(game_over_stage);
            return;
        }
        break;
      }

      case PLAYING: {
        game_stage_action_t action = handle_playing_stage(playing_stage);
        switch (action) {
          case PROGRESS:
            game_stage = GAME_OVER;
            break;
          case QUIT:
            destroy_intro_stage(intro_stage);
            destroy_playing_stage(playing_stage);
            destroy_game_over_stage(game_over_stage);
            return;
        }
        break;
      }

      case GAME_OVER: {
        game_stage_action_t action = handle_game_over_stage(game_over_stage);
        switch (action) {
          case PROGRESS:
            game_stage = PLAYING;
            reset_game(game);
            break;
          case QUIT:
            destroy_intro_stage(intro_stage);
            destroy_playing_stage(playing_stage);
            destroy_game_over_stage(game_over_stage);
            return;
        }
        break;
      }
    }
  }
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
  LOG_INFO_FMT("Audio Volume: %d/128", game_settings.volume);
  LOG_INFO("==============================");

  game_t game = init_game(game_settings);

  run_game(&game);

  terminate_game(&game);

  return 0;
}
