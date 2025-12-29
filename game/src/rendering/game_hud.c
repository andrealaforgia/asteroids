#include "game_hud.h"

#include "geometry.h"
#include "sprites.h"
#include "text.h"

void init_game_hud(game_hud_ptr hud, game_ptr game,
                   graphics_context_ptr graphics_context) {
  hud->game = game;
  hud->graphics_context = graphics_context;
  hud->fps_tracker = create_fps_tracker();
}

void reset_game_hud(game_hud_ptr hud) {
  hud->fps_tracker = create_fps_tracker();
}

static ALWAYS_INLINE void show_lives(game_hud_ptr hud, int ship_scale) {
  render_lives(hud->graphics_context,
               point(hud->graphics_context->screen_center.x -
                         ship_scale * 12 * hud->game->lives - 10,
                     ship_scale * 12 + 5),
               hud->game->lives);
}

static ALWAYS_INLINE void show_score(game_hud_ptr hud) {
  text_dimensions_t text_dimensions = calculate_text_dimensions("9", 10);
  write_number(hud->graphics_context,
               point(hud->graphics_context->screen_center.x,
                     text_dimensions.height + 5),
               hud->game->score, 10);
}

static ALWAYS_INLINE void show_fps_if_required(game_hud_ptr hud) {
  track_fps(&hud->fps_tracker);
  if (hud->game->settings.show_fps) {
    char fps_text[10] = {0};
    format_fps(&hud->fps_tracker, fps_text, sizeof fps_text);
    text_dimensions_t fps_text_dimensions =
        calculate_text_dimensions(fps_text, 5);
    write_text(hud->graphics_context, fps_text,
               point(5, 5 + fps_text_dimensions.height), 5, COLOR_WHITE);
  }
}

static ALWAYS_INLINE void show_sound_indicator(game_hud_ptr hud) {
  int base_x = 10;
  int base_y = 10;
  color_t color = hud->game->settings.volume > 0 ? COLOR_GREEN : COLOR_RED;

  // Draw speaker cone (trapezoid)
  draw_line(hud->graphics_context, base_x, base_y + 3, base_x, base_y + 9,
            color);
  draw_line(hud->graphics_context, base_x, base_y + 3, base_x + 3, base_y,
            color);
  draw_line(hud->graphics_context, base_x, base_y + 9, base_x + 3,
            base_y + 12, color);
  draw_line(hud->graphics_context, base_x + 3, base_y, base_x + 3,
            base_y + 12, color);

  if (hud->game->settings.volume > 0) {
    // Draw sound waves (3 arcs represented as lines)
    draw_line(hud->graphics_context, base_x + 5, base_y + 2, base_x + 6,
              base_y + 1, color);
    draw_line(hud->graphics_context, base_x + 5, base_y + 10, base_x + 6,
              base_y + 11, color);

    draw_line(hud->graphics_context, base_x + 7, base_y + 1, base_x + 8,
              base_y, color);
    draw_line(hud->graphics_context, base_x + 7, base_y + 11, base_x + 8,
              base_y + 12, color);

    draw_line(hud->graphics_context, base_x + 9, base_y, base_x + 10,
              base_y - 1, color);
    draw_line(hud->graphics_context, base_x + 9, base_y + 12, base_x + 10,
              base_y + 13, color);
  } else {
    // Draw red cross (X) over the speaker
    draw_line(hud->graphics_context, base_x + 5, base_y + 2, base_x + 11,
              base_y + 10, COLOR_RED);
    draw_line(hud->graphics_context, base_x + 11, base_y + 2, base_x + 5,
              base_y + 10, COLOR_RED);
  }
}

void render_hud(game_hud_ptr hud, int ship_scale) {
  show_lives(hud, ship_scale);
  show_score(hud);
  show_fps_if_required(hud);
  show_sound_indicator(hud);
}
