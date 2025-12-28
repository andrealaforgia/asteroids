#include "audio.h"

#include <SDL_mixer.h>
#include <stdio.h>

#include "inline.h"
#include "logger.h"

#define BANG_LARGE_INDEX 0
#define BANG_MEDIUM_INDEX 1
#define BANG_SMALL_INDEX 2
#define BEAT1_INDEX 3
#define BEAT2_INDEX 4
#define EXTRA_SHIP_INDEX 5
#define FIRE_INDEX 6
#define SAUCER_BIG_INDEX 7
#define SAUCER_SMALL_INDEX 8
#define THRUST_INDEX 9
#define GAME_OVER_INDEX 10

#define BANG_LARGE_WAV "./sounds/bangLarge.wav"
#define BANG_MEDIUM_WAV "./sounds/bangMedium.wav"
#define BANG_SMALL_WAV "./sounds/bangSmall.wav"
#define BEAT1_WAV "./sounds/beat1.wav"
#define BEAT2_WAV "./sounds/beat2.wav"
#define EXTRA_SHIP_WAV "./sounds/extraShip.wav"
#define FIRE_WAV "./sounds/fire.wav"
#define SAUCER_SMALL_WAV "./sounds/saucerSmall.wav"
#define SAUCER_BIG_WAV "./sounds/saucerBig.wav"
#define THRUST_WAV "./sounds/thrust.wav"
#define GAME_OVER_WAV "./sounds/gameOver.wav"

// Helper function to load sound with error checking
static Mix_Chunk *load_sound(const char *path) {
  Mix_Chunk *chunk = Mix_LoadWAV(path);
  if (!chunk) {
    LOG_MIX_ERROR(path);
    LOG_WARN("Game will continue without this sound effect");
  }
  return chunk;
}

audio_context_t init_audio_context(void) {
  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) < 0) {
    LOG_MIX_ERROR("Mix_OpenAudio");
  }
  audio_context_t audio_context;
  audio_context.chunks[BANG_LARGE_INDEX] = load_sound(BANG_LARGE_WAV);
  audio_context.chunks[BANG_MEDIUM_INDEX] = load_sound(BANG_MEDIUM_WAV);
  audio_context.chunks[BANG_SMALL_INDEX] = load_sound(BANG_SMALL_WAV);
  audio_context.chunks[BEAT1_INDEX] = load_sound(BEAT1_WAV);
  audio_context.chunks[BEAT2_INDEX] = load_sound(BEAT2_WAV);
  audio_context.chunks[EXTRA_SHIP_INDEX] = load_sound(EXTRA_SHIP_WAV);
  audio_context.chunks[FIRE_INDEX] = load_sound(FIRE_WAV);
  audio_context.chunks[SAUCER_BIG_INDEX] = load_sound(SAUCER_BIG_WAV);
  audio_context.chunks[SAUCER_SMALL_INDEX] = load_sound(SAUCER_SMALL_WAV);
  audio_context.chunks[THRUST_INDEX] = load_sound(THRUST_WAV);
  audio_context.chunks[GAME_OVER_INDEX] = load_sound(GAME_OVER_WAV);

  // Amount of channels (Max amount of sounds playing at the same time)
  int channels = Mix_AllocateChannels(256);
  if (channels < 256) {
    LOG_WARN("Could not allocate 256 audio channels");
    LOG_INFO_FMT("Allocated %d channels instead", channels);
  }

  // Volume is in the range 0-128, where 128 is full volume
  int volume = 32;
  Mix_Volume(
      -1,
      volume);  // -1 affects all channels, or you can use a specific channel ID

  return audio_context;
}

ALWAYS_INLINE void play_bang_large(const audio_context_ptr audio_context) {
  if (audio_context->chunks[BANG_LARGE_INDEX]) {
    Mix_PlayChannel(-1, audio_context->chunks[BANG_LARGE_INDEX], 0);
  }
}

ALWAYS_INLINE void play_bang_medium(const audio_context_ptr audio_context) {
  if (audio_context->chunks[BANG_MEDIUM_INDEX]) {
    Mix_PlayChannel(-1, audio_context->chunks[BANG_MEDIUM_INDEX], 0);
  }
}

ALWAYS_INLINE void play_bang_small(const audio_context_ptr audio_context) {
  if (audio_context->chunks[BANG_SMALL_INDEX]) {
    Mix_PlayChannel(-1, audio_context->chunks[BANG_SMALL_INDEX], 0);
  }
}

ALWAYS_INLINE void play_beat1(const audio_context_ptr audio_context) {
  if (audio_context->chunks[BEAT1_INDEX]) {
    Mix_PlayChannel(-1, audio_context->chunks[BEAT1_INDEX], 0);
  }
}

ALWAYS_INLINE void play_beat2(const audio_context_ptr audio_context) {
  if (audio_context->chunks[BEAT2_INDEX]) {
    Mix_PlayChannel(-1, audio_context->chunks[BEAT2_INDEX], 0);
  }
}

ALWAYS_INLINE void play_extra_ship(const audio_context_ptr audio_context) {
  if (audio_context->chunks[EXTRA_SHIP_INDEX]) {
    Mix_PlayChannel(-1, audio_context->chunks[EXTRA_SHIP_INDEX], 0);
  }
}

ALWAYS_INLINE void play_fire(const audio_context_ptr audio_context) {
  if (audio_context->chunks[FIRE_INDEX]) {
    Mix_PlayChannel(-1, audio_context->chunks[FIRE_INDEX], 0);
  }
}

ALWAYS_INLINE void play_saucer_big(const audio_context_ptr audio_context) {
  if (audio_context->chunks[SAUCER_BIG_INDEX]) {
    Mix_PlayChannel(-1, audio_context->chunks[SAUCER_BIG_INDEX], 0);
  }
}

ALWAYS_INLINE void play_saucer_small(const audio_context_ptr audio_context) {
  if (audio_context->chunks[SAUCER_SMALL_INDEX]) {
    Mix_PlayChannel(-1, audio_context->chunks[SAUCER_SMALL_INDEX], 0);
  }
}

ALWAYS_INLINE void play_thrust(const audio_context_ptr audio_context) {
  if (audio_context->chunks[THRUST_INDEX]) {
    Mix_PlayChannel(-1, audio_context->chunks[THRUST_INDEX], 0);
  }
}

ALWAYS_INLINE void play_game_over(const audio_context_ptr audio_context) {
  if (audio_context->chunks[GAME_OVER_INDEX]) {
    Mix_PlayChannel(-1, audio_context->chunks[GAME_OVER_INDEX], 0);
  }
}

void terminate_audio_context(const audio_context_ptr audio_context) {
  for (int i = 0; i < 10; i++) {
    Mix_FreeChunk(audio_context->chunks[i]);
  }
  Mix_CloseAudio();
}
