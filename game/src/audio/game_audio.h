#ifndef GAME_SRC_AUDIO_GAME_AUDIO_H_
#define GAME_SRC_AUDIO_GAME_AUDIO_H_

#include "audio.h"
#include "event_system.h"

// Asteroids-specific sound indices
typedef enum {
  SOUND_BANG_LARGE = 0,
  SOUND_BANG_MEDIUM = 1,
  SOUND_BANG_SMALL = 2,
  SOUND_BEAT1 = 3,
  SOUND_BEAT2 = 4,
  SOUND_EXTRA_SHIP = 5,
  SOUND_FIRE = 6,
  SOUND_SAUCER_BIG = 7,
  SOUND_SAUCER_SMALL = 8,
  SOUND_THRUST = 9,
  SOUND_GAME_OVER = 10,
  SOUND_SHIP_LOST = 11,
  SOUND_COUNT = 12
} game_sound_t;

// Initialize all Asteroids sounds
void init_game_audio(audio_context_ptr audio_context);

// Asteroids-specific sound playback functions
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
void play_ship_lost(const audio_context_ptr audio_context);

// Event subscriber functions
void subscribe_audio_events(event_system_ptr events,
                            audio_context_ptr audio);

#endif  // GAME_SRC_AUDIO_GAME_AUDIO_H_
