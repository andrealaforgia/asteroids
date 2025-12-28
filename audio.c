#include "audio.h"

#include <SDL.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

// Sound file names (relative to executable directory)
#define BANG_LARGE_WAV "game/assets/sounds/bangLarge.wav"
#define BANG_MEDIUM_WAV "game/assets/sounds/bangMedium.wav"
#define BANG_SMALL_WAV "game/assets/sounds/bangSmall.wav"
#define BEAT1_WAV "game/assets/sounds/beat1.wav"
#define BEAT2_WAV "game/assets/sounds/beat2.wav"
#define EXTRA_SHIP_WAV "game/assets/sounds/extraShip.wav"
#define FIRE_WAV "game/assets/sounds/fire.wav"
#define SAUCER_SMALL_WAV "game/assets/sounds/saucerSmall.wav"
#define SAUCER_BIG_WAV "game/assets/sounds/saucerBig.wav"
#define THRUST_WAV "game/assets/sounds/thrust.wav"
#define GAME_OVER_WAV "game/assets/sounds/gameOver.wav"

// Construct full path to a sound file
static char *get_sound_path(const char *base_path, const char *sound_file) {
  size_t path_len = strlen(base_path) + strlen(sound_file) + 1;
  char *full_path = malloc(path_len);
  if (full_path) {
    snprintf(full_path, path_len, "%s%s", base_path, sound_file);
  }
  return full_path;
}

// Helper function to load sound with error checking
static Mix_Chunk *load_sound(const char *base_path, const char *sound_file) {
  char *full_path = get_sound_path(base_path, sound_file);
  if (!full_path) {
    LOG_WARN("Failed to allocate memory for sound path");
    return NULL;
  }

  Mix_Chunk *chunk = Mix_LoadWAV(full_path);
  if (!chunk) {
    LOG_MIX_ERROR(full_path);
    LOG_WARN("Game will continue without this sound effect");
  }

  free(full_path);
  return chunk;
}

audio_context_t init_audio_context(int volume) {
  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) < 0) {
    LOG_MIX_ERROR("Mix_OpenAudio");
  }

  // Get the base path for the executable
  char *base_path = SDL_GetBasePath();
  if (!base_path) {
    LOG_SDL_ERROR("SDL_GetBasePath");
    LOG_WARN("Using current directory for sound files");
    base_path = SDL_strdup("./");
  }

  audio_context_t audio_context;
  audio_context.chunks[BANG_LARGE_INDEX] = load_sound(base_path, BANG_LARGE_WAV);
  audio_context.chunks[BANG_MEDIUM_INDEX] =
      load_sound(base_path, BANG_MEDIUM_WAV);
  audio_context.chunks[BANG_SMALL_INDEX] = load_sound(base_path, BANG_SMALL_WAV);
  audio_context.chunks[BEAT1_INDEX] = load_sound(base_path, BEAT1_WAV);
  audio_context.chunks[BEAT2_INDEX] = load_sound(base_path, BEAT2_WAV);
  audio_context.chunks[EXTRA_SHIP_INDEX] = load_sound(base_path, EXTRA_SHIP_WAV);
  audio_context.chunks[FIRE_INDEX] = load_sound(base_path, FIRE_WAV);
  audio_context.chunks[SAUCER_BIG_INDEX] = load_sound(base_path, SAUCER_BIG_WAV);
  audio_context.chunks[SAUCER_SMALL_INDEX] =
      load_sound(base_path, SAUCER_SMALL_WAV);
  audio_context.chunks[THRUST_INDEX] = load_sound(base_path, THRUST_WAV);
  audio_context.chunks[GAME_OVER_INDEX] = load_sound(base_path, GAME_OVER_WAV);

  SDL_free(base_path);

  // Amount of channels (Max amount of sounds playing at the same time)
  int channels = Mix_AllocateChannels(256);
  if (channels < 256) {
    LOG_WARN("Could not allocate 256 audio channels");
    LOG_INFO_FMT("Allocated %d channels instead", channels);
  }

  // Volume is in the range 0-128, where 128 is full volume
  Mix_Volume(-1, volume);  // -1 affects all channels
  LOG_INFO_FMT("Audio volume set to %d/128", volume);

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
  // Free all 11 sound chunks (indices 0-10)
  for (int i = 0; i < 11; i++) {
    if (audio_context->chunks[i]) {
      Mix_FreeChunk(audio_context->chunks[i]);
    }
  }
  Mix_CloseAudio();
}
