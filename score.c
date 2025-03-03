#include "score.h"

#include "game.h"

ALWAYS_INLINE void score(const game_ptr game, int score) {
  if (game->score + score > MAX_SCORE) {
    game->score = MAX_SCORE;
    return;
  }
  game->score += score;
}

ALWAYS_INLINE void score_large_asteroid(const game_ptr game) {
  score(game, LARGE_ASTEROID_SCORE);
}

ALWAYS_INLINE void score_medium_asteroid(const game_ptr game) {
  score(game, MEDIUM_ASTEROID_SCORE);
}

ALWAYS_INLINE void score_small_asteroid(const game_ptr game) {
  score(game, SMALL_ASTEROID_SCORE);
}

ALWAYS_INLINE void score_large_saucer(const game_ptr game) {
  score(game, LARGE_SAUCER_SCORE);
}

ALWAYS_INLINE void score_small_saucer(const game_ptr game) {
  score(game, SMALL_SAUCER_SCORE);
}
