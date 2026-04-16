/**
*                      This file is part of:
*                          Libretti
*
* Libretti
*
* Structures the compiler, and waveforms generator
* procedures by default for simplicity.
*
* Copyright (c) 2017 Jean-Louis Haywood. All rights reserved.
*/

#ifndef Libretti_h
#define Libretti_h

#include <stdint.h>

#include "composition.h"
#include "binary.h"
#include "notes.h"
#include "waveforms.h"
#include "playback.h"
#include "audio_clip.h"

#include "validation.h"

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef LIBRETTI_API
	#if defined (_WIN32)
		#if defined(_LIBRETTI_EXPORTS)
			#if defined(_LIBRETTI_EXPORTS_BUILD)
				#define  LIBRETTI_API __declspec(dllexport)
			#else
				#define  LIBRETTI_API __declspec(dllimport)
			#endif
		#else
			#define LIBRETTI_API
		#endif
	#else
		#define LIBRETTI_API
	#endif
#endif

/*Read only audio, output note wave streams, playback runtime.*/
typedef struct
{
	lb_Composition* composition;
	lb_Playback* playback;
	int id;
}lb_Libretti;

/*Allocates and returns a Libretti without adding to callback. */
LIBRETTI_API lb_Libretti* lb_create_libretti(const char* filename);

/*Allocates and returns an empty Libretti without adding to callback. */
LIBRETTI_API lb_Libretti* lb_create_empty_libretti();

/*Returns an composition struct with a compiled script.*/
LIBRETTI_API lb_Composition* lb_create_composition(const char* filename);

/*Returns an empty composition struct.*/
LIBRETTI_API lb_Composition* lb_create_empty_composition();

/*Returns playback data with resetted time and device index for SDL audio.*/
LIBRETTI_API lb_Playback* lb_create_playback();

/*Loads script and validates it against the language specification, returns validation codes.*/
LIBRETTI_API int lb_validate_script_file(const char* filename);

/*Updates existing composition with compiled script.*/
LIBRETTI_API void lb_compile_composition_from_script_file(lb_Composition* composition, const char* filename);

/*Updates playback from composition according to the update playback time.*/
LIBRETTI_API void lb_update_playback(lb_Playback* playback, lb_Composition* composition, int sample_rate);

/*Extract as many simultaneous notes that matches the composition track count, at a given playback.*/
LIBRETTI_API void lb_update_notes_from_composition(lb_Note current_notes[], lb_Composition* composition, lb_Playback* playback);

/*Updates note waves with a PCM representation of the encoded notes.*/
LIBRETTI_API void lb_update_waveform_from_notes(lb_Waveforms* waveforms, lb_Note current_notes[], int sample_rate);

/*Updates the Libretti's playback with a delta in seconds.*/
LIBRETTI_API void lb_increment_play_time(lb_Libretti* libretti, float delta_time);

/*Updates an existing Libretti with a compiled script.*/
LIBRETTI_API void lb_load(lb_Libretti* libretti, const char* filename);

/*Allows the Libretti's playback to increment time.*/
LIBRETTI_API void lb_play(lb_Libretti* libretti);

/*Stops the Libretti's playback from incrementing time.*/
LIBRETTI_API void lb_pause(lb_Libretti* libretti);

/*Sets the Libretti's playback to 0.0s.*/
LIBRETTI_API void lb_reset(lb_Libretti* libretti);

/*Resets and Pauses the Libretti's playback.*/
LIBRETTI_API void lb_stop(lb_Libretti* libretti);

/*Writes a file streamed in unsigned 8-bits.*/
LIBRETTI_API void lb_save_binary_u8_to_file(lb_BinaryU8* binary, const char* filename);

/*Writes a file streamed in signed 16-bits.*/
LIBRETTI_API void lb_save_binary_s16_to_file(lb_BinaryS16* binary, const char* filename);

/*Appends a file streamed in unsigned 8-bits.*/
LIBRETTI_API void lb_append_binary_u8_to_file(lb_BinaryU8* binary, const char* filename);

/*Appends a file streamed in signed 16-bits.*/
LIBRETTI_API void lb_append_binary_s16_to_file(lb_BinaryS16* binary, const char* filename);

/*Decodes an audio struct to a PCM stream.*/
LIBRETTI_API lb_AudioClip lb_synthesize_audio_clip(lb_Libretti* libretti, uint32_t sample_rate, uint8_t channel_count);

LIBRETTI_API bool lb_save_audio_clip_to_wav_file(lb_AudioClip* audio_clip, const char* filename);

/*Deletes playback memory allocation.*/
LIBRETTI_API void lb_free_playback(lb_Playback* playback);

/*Deletes audio memory allocation.*/
LIBRETTI_API void lb_free_composition(lb_Composition* composition);

/*Deletes memory allocation of playback, note waves and audio, and the container Libretti.*/
LIBRETTI_API void lb_free_libretti(lb_Libretti* libretti);

#ifdef __cplusplus
}
#endif

#endif /*Libretti_h*/