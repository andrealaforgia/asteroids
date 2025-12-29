#ifndef ENGINE_CORE_AUDIO_AUDIO_H_
#define ENGINE_CORE_AUDIO_AUDIO_H_

#include <SDL_mixer.h>
#include <stdbool.h>

typedef struct {
  Mix_Chunk** chunks;
  int max_sounds;
} audio_context_t, *audio_context_ptr;

// Initialize audio context with maximum number of sounds and volume (0-128)
audio_context_t init_audio_context(int max_sounds, int volume);

// Load a sound file at the specified index
bool load_sound(audio_context_ptr audio_context, int index,
                const char* base_path, const char* sound_file);

// Play a sound at the specified index
void play_sound(const audio_context_ptr audio_context, int index);

// Clean up and free all audio resources
void terminate_audio_context(const audio_context_ptr audio_context);

#endif  // ENGINE_CORE_AUDIO_AUDIO_H_
