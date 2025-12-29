#include "game_audio.h"

#include <SDL.h>

#include "asteroid.h"
#include "audio.h"
#include "event_system.h"
#include "game_events.h"
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

void play_bang_large(const audio_context_ptr audio_context) {
  play_sound(audio_context, SOUND_BANG_LARGE);
}

void play_bang_medium(const audio_context_ptr audio_context) {
  play_sound(audio_context, SOUND_BANG_MEDIUM);
}

void play_bang_small(const audio_context_ptr audio_context) {
  play_sound(audio_context, SOUND_BANG_SMALL);
}

void play_beat1(const audio_context_ptr audio_context) {
  play_sound(audio_context, SOUND_BEAT1);
}

void play_beat2(const audio_context_ptr audio_context) {
  play_sound(audio_context, SOUND_BEAT2);
}

void play_extra_ship(const audio_context_ptr audio_context) {
  play_sound(audio_context, SOUND_EXTRA_SHIP);
}

void play_fire(const audio_context_ptr audio_context) {
  play_sound(audio_context, SOUND_FIRE);
}

void play_saucer_big(const audio_context_ptr audio_context) {
  play_sound(audio_context, SOUND_SAUCER_BIG);
}

void play_saucer_small(const audio_context_ptr audio_context) {
  play_sound(audio_context, SOUND_SAUCER_SMALL);
}

void play_thrust(const audio_context_ptr audio_context) {
  play_sound(audio_context, SOUND_THRUST);
}

void play_game_over(const audio_context_ptr audio_context) {
  play_sound(audio_context, SOUND_GAME_OVER);
}

void play_ship_lost(const audio_context_ptr audio_context) {
  play_sound(audio_context, SOUND_SHIP_LOST);
}

/* ---- ==== Event Subscribers ==== ---- */

static void on_asteroid_destroyed(const game_event_t* event, void* user_data) {
  audio_context_ptr audio = (audio_context_ptr)user_data;
  asteroid_destroyed_data_t* data = (asteroid_destroyed_data_t*)event->data;

  SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
              "Asteroid destroyed event received (scale=%d)", data->scale);

  switch (data->scale) {
    case LARGE_ASTEROID_SCALE:
      SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Playing bang large");
      play_bang_large(audio);
      break;
    case MEDIUM_ASTEROID_SCALE:
      SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Playing bang medium");
      play_bang_medium(audio);
      break;
    case SMALL_ASTEROID_SCALE:
      SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Playing bang small");
      play_bang_small(audio);
      break;
  }
}

static void on_ship_destroyed(const game_event_t* event, void* user_data) {
  audio_context_ptr audio = (audio_context_ptr)user_data;
  (void)event;  // Unused
  play_ship_lost(audio);
}

static void on_saucer_destroyed(const game_event_t* event, void* user_data) {
  audio_context_ptr audio = (audio_context_ptr)user_data;
  saucer_destroyed_data_t* data = (saucer_destroyed_data_t*)event->data;

  if (data->is_big) {
    play_saucer_big(audio);
  } else {
    play_saucer_small(audio);
  }
}

void subscribe_audio_events(event_system_ptr events,
                            audio_context_ptr audio) {
  subscribe(events, GAME_EVENT_ASTEROID_DESTROYED, on_asteroid_destroyed,
            audio);
  subscribe(events, GAME_EVENT_SHIP_DESTROYED, on_ship_destroyed, audio);
  subscribe(events, GAME_EVENT_SAUCER_DESTROYED, on_saucer_destroyed, audio);
}
