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

void render_hud(game_hud_ptr hud, int ship_scale) {
  show_lives(hud, ship_scale);
  show_score(hud);
  show_fps_if_required(hud);
}
