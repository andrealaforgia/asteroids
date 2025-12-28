#ifndef SCORE_H_
#define SCORE_H_

#include "game.h"

#define MAX_SCORE 999999999
#define LARGE_ASTEROID_SCORE 20
#define MEDIUM_ASTEROID_SCORE 50
#define SMALL_ASTEROID_SCORE 100
#define LARGE_SAUCER_SCORE 200
#define SMALL_SAUCER_SCORE 1000

void score(const game_ptr game, int score);
void score_large_asteroid(const game_ptr game);
void score_medium_asteroid(const game_ptr game);
void score_small_asteroid(const game_ptr game);
void score_large_saucer(const game_ptr game);
void score_small_saucer(const game_ptr game);

#endif  // SCORE_H_
