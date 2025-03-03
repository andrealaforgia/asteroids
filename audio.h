#ifndef AUDIO_H_
#define AUDIO_H_

#include <SDL_mixer.h>

typedef struct {
  Mix_Chunk *chunks[11];
} audio_context_t, *audio_context_ptr;

audio_context_t init_audio_context(void);
void play_bang_large(const audio_context_ptr audio_context);
void play_bang_medium(const audio_context_ptr audio_context);
void play_bang_small(const audio_context_ptr audio_context);
void play_beat1(const audio_context_ptr audio_context);
void play_beat2(const audio_context_ptr audio_context);
void play_extra_ship(const audio_context_ptr audio_context);
void play_fire(const audio_context_ptr audio_context);
void play_saucer_big(const audio_context_ptr audio_context);
void play_saucer_small(const audio_context_ptr audio_context);
void play_thrust(const audio_context_ptr audio_context);
void play_game_over(const audio_context_ptr audio_context);
void terminate_audio_context(const audio_context_ptr audio_context);

#endif  // AUDIO_H_
