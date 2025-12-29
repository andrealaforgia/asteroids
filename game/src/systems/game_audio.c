#include "game_audio.h"

#include <SDL.h>

#include "audio.h"
#include "inline.h"

// Sound file paths (relative to executable directory)
#define BANG_LARGE_WAV "game/assets/sounds/bang_large.wav"
#define BANG_MEDIUM_WAV "game/assets/sounds/bang_medium.wav"
#define BANG_SMALL_WAV "game/assets/sounds/bang_small.wav"
#define BEAT1_WAV "game/assets/sounds/beat1.wav"
#define BEAT2_WAV "game/assets/sounds/beat2.wav"
#define EXTRA_SHIP_WAV "game/assets/sounds/extra_ship.wav"
#define FIRE_WAV "game/assets/sounds/fire.wav"
#define SAUCER_SMALL_WAV "game/assets/sounds/saucer_small.wav"
#define SAUCER_BIG_WAV "game/assets/sounds/saucer_big.wav"
#define THRUST_WAV "game/assets/sounds/thrust.wav"
#define GAME_OVER_WAV "game/assets/sounds/game_over.wav"
#define SHIP_LOST_MP3 "game/assets/sounds/ship_lost.mp3"

void init_game_audio(audio_context_ptr audio_context) {
  // Get the base path for the executable
  char* base_path = SDL_GetBasePath();
  if (!base_path) {
    base_path = SDL_strdup("./");
  }

  // Load all Asteroids sounds
  load_sound(audio_context, SOUND_BANG_LARGE, base_path, BANG_LARGE_WAV);
  load_sound(audio_context, SOUND_BANG_MEDIUM, base_path, BANG_MEDIUM_WAV);
  load_sound(audio_context, SOUND_BANG_SMALL, base_path, BANG_SMALL_WAV);
  load_sound(audio_context, SOUND_BEAT1, base_path, BEAT1_WAV);
  load_sound(audio_context, SOUND_BEAT2, base_path, BEAT2_WAV);
  load_sound(audio_context, SOUND_EXTRA_SHIP, base_path, EXTRA_SHIP_WAV);
  load_sound(audio_context, SOUND_FIRE, base_path, FIRE_WAV);
  load_sound(audio_context, SOUND_SAUCER_BIG, base_path, SAUCER_BIG_WAV);
  load_sound(audio_context, SOUND_SAUCER_SMALL, base_path, SAUCER_SMALL_WAV);
  load_sound(audio_context, SOUND_THRUST, base_path, THRUST_WAV);
  load_sound(audio_context, SOUND_GAME_OVER, base_path, GAME_OVER_WAV);
  load_sound(audio_context, SOUND_SHIP_LOST, base_path, SHIP_LOST_MP3);

  SDL_free(base_path);
}

ALWAYS_INLINE void play_bang_large(const audio_context_ptr audio_context) {
  play_sound(audio_context, SOUND_BANG_LARGE);
}

ALWAYS_INLINE void play_bang_medium(const audio_context_ptr audio_context) {
  play_sound(audio_context, SOUND_BANG_MEDIUM);
}

ALWAYS_INLINE void play_bang_small(const audio_context_ptr audio_context) {
  play_sound(audio_context, SOUND_BANG_SMALL);
}

ALWAYS_INLINE void play_beat1(const audio_context_ptr audio_context) {
  play_sound(audio_context, SOUND_BEAT1);
}

ALWAYS_INLINE void play_beat2(const audio_context_ptr audio_context) {
  play_sound(audio_context, SOUND_BEAT2);
}

ALWAYS_INLINE void play_extra_ship(const audio_context_ptr audio_context) {
  play_sound(audio_context, SOUND_EXTRA_SHIP);
}

ALWAYS_INLINE void play_fire(const audio_context_ptr audio_context) {
  play_sound(audio_context, SOUND_FIRE);
}

ALWAYS_INLINE void play_saucer_big(const audio_context_ptr audio_context) {
  play_sound(audio_context, SOUND_SAUCER_BIG);
}

ALWAYS_INLINE void play_saucer_small(const audio_context_ptr audio_context) {
  play_sound(audio_context, SOUND_SAUCER_SMALL);
}

ALWAYS_INLINE void play_thrust(const audio_context_ptr audio_context) {
  play_sound(audio_context, SOUND_THRUST);
}

ALWAYS_INLINE void play_game_over(const audio_context_ptr audio_context) {
  play_sound(audio_context, SOUND_GAME_OVER);
}

ALWAYS_INLINE void play_ship_lost(const audio_context_ptr audio_context) {
  play_sound(audio_context, SOUND_SHIP_LOST);
}
