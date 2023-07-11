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

#include "Composition.h"
#include "Binary.h"
#include "Notes.h"
#include "Waveforms.h"
#include "Playback.h"

#include "Validation.h"

#ifdef __cplusplus
extern "C"
{
#endif

#if defined (_WIN32)
#if defined(_LIBRETTI_EXPORTS)
#define  LIBRETTI_API __declspec(dllexport)
#else
#define  LIBRETTI_API __declspec(dllimport)
#endif
#else
#define LIBRETTI_API
#endif

	/*Read only audio, output note wave streams, playback runtime.*/
	typedef struct
	{
		lb_Composition* composition;
		lb_Playback* playback;
		int id;
	}lb_Libretti;

	/*Allocates and returns a Libretti without adding to callback. */
	LIBRETTI_API void lb_initialize();

	/*Allocates and returns a Libretti without adding to callback. */
	LIBRETTI_API lb_Libretti* lb_createLibretti(const char* filename);

	/*Allocates and returns an empty Libretti without adding to callback. */
	LIBRETTI_API lb_Libretti* lb_createEmptyLibretti();

	/*Returns an composition struct with a compiled script.*/
	LIBRETTI_API lb_Composition* lb_createComposition(const char* filename);

	/*Returns an empty composition struct.*/
	LIBRETTI_API lb_Composition* lb_createEmptyComposition();

	/*Returns playback data with resetted time and device index for SDL audio.*/
	LIBRETTI_API lb_Playback* lb_createPlayback();

	/*Updates the global callback list to playback Libretti simultaneously.*/
	LIBRETTI_API void lb_addLibrettiToCallback(lb_Libretti* libretti);

	/*Removes an active Libretti from the callback using its ID.*/
	LIBRETTI_API void lb_removeLibrettiFromCallback(int librettiID);

	/*Allocates and returns a Libretti with the global callback list updated.*/
	LIBRETTI_API lb_Libretti* lb_createAndAddLibrettiToCallback(const char* filename);

	/*Allocates and returns an empty Libretti with the global callback list updated.*/
	LIBRETTI_API lb_Libretti* lb_createAndAddEmptyLibrettiToCallback();

	/*Loads script and validates it against the language specification, returns validation codes.*/
	LIBRETTI_API int lb_validateScriptFile(const char* filename);

	/*Updates existing composition with compiled script.*/
	LIBRETTI_API void lb_compileCompositionFromScriptFile(lb_Composition* composition, const char* filename);

	/*Updates playback from composition according to the update playback time.*/
	LIBRETTI_API void lb_updatePlayback(lb_Playback* playback, lb_Composition* composition);

	/*Extract as many simultaneous notes that matches the composition track count, at a given playback.*/
	LIBRETTI_API void lb_updateNotesFromComposition(lb_Note currentNotes[], lb_Composition* composition, lb_Playback* playback);

	/*Updates note waves with a PCM representation of the encoded notes.*/
	LIBRETTI_API void lb_updateWaveformFromNotes(lb_Waveforms* waveforms, lb_Note currentNotes[], uint8_t trackCount);

	/*Updates the Libretti's playback with a delta in seconds.*/
	LIBRETTI_API void lb_incrementPlayTime(lb_Libretti* libretti, float deltaTime_s);

	/*Updates the times of all Librettis in the global callback list.*/
	LIBRETTI_API void lb_incrementAllPlayTimes(float deltaTime_s);

	/*Updates an existing Libretti with a compiled script.*/
	LIBRETTI_API void lb_load(lb_Libretti* libretti, const char* filename);

	/*Allows the Libretti's playback to increment time.*/
	LIBRETTI_API void lb_play(lb_Libretti* libretti);

	/*Plays a sound in the WaveformGenerator until the duration is reached.
	 * key			- the tone of the note
	 * dynamic		- the volume or amplitude of the note
	 * panning		- the direction of the audio in the L/R speaker configuration
	 * timbre		- the type of instrument used to render the sound
	 * articulation	- the manner in which the key is released.
	 * Each parameter must be selected from its corresponding enum*/
	LIBRETTI_API lb_Libretti* lb_play_note_for(
		uint16_t key,
		uint16_t dynamic,
		uint8_t panning,
		uint8_t timbre,
		uint8_t articulation,
		float duration);

	/*Infinitely plays a sound in the WaveformGenerator.
	 * key			- the tone of the note
	 * dynamic		- the volume or amplitude of the note
	 * panning		- the direction of the audio in the L/R speaker configuration
	 * timbre		- the type of instrument used to render the sound
	 * Each parameter must be selected from its corresponding enum*/
	LIBRETTI_API lb_Libretti* lb_play_note(
		uint16_t key,
		uint16_t dynamic,
		uint8_t panning,
		uint8_t timbre);

	/*Plays a sound with the minimal amount of info needed for most common case
	 * key			- the tone of the note
	 * dynamic		- the volume or amplitude of the note
	 * Each parameter must be selected from its corresponding enum*/
	LIBRETTI_API lb_Libretti* lb_play_simple_note_for(
		uint16_t key,
		uint16_t dynamic,
		float duration);

	/*Infinitely plays a sound with the minimal amount of info needed for most common case
	 * key			- the tone of the note
	 * dynamic		- the volume or amplitude of the note
	 * Each parameter must be selected from its corresponding enum*/
	LIBRETTI_API lb_Libretti* lb_play_simple_note(
		uint16_t key,
		uint16_t dynamic);

	/*Stops the Libretti's playback from incrementing time.*/
	LIBRETTI_API void lb_pause(lb_Libretti* libretti);

	/*Sets the Libretti's playback to 0.0s.*/
	LIBRETTI_API void lb_reset(lb_Libretti* libretti);

	/*Resets and Pauses the Libretti's playback.*/
	LIBRETTI_API void lb_stop(lb_Libretti* libretti);

	/*Plays all Librettis in the global callback list.*/
	LIBRETTI_API void lb_playAll();

	/*Pause all Librettis in the global callback list.*/
	LIBRETTI_API void lb_pauseAll();

	/*Resets all Librettis in the global callback list.*/
	LIBRETTI_API void lb_resetAll();

	/*Stops all Librettis in the global callback list.*/
	LIBRETTI_API void lb_stopAll();

	/*Allocates and returns a region in memory that captures the incoming stream recording.*/
	LIBRETTI_API lb_BinaryS16* lb_getAudioCaptureStreamBuffer();

	/*Writes a file streamed in unsigned 8-bits.*/
	LIBRETTI_API void lb_saveBinaryU8ToFile(lb_BinaryU8* binary, const char* filename);

	/*Writes a file streamed in signed 16-bits.*/
	LIBRETTI_API void lb_saveBinaryS16ToFile(lb_BinaryS16* binary, const char* filename);

	/*Appends a file streamed in unsigned 8-bits.*/
	LIBRETTI_API void lb_appendBinaryU8ToFile(lb_BinaryU8* binary, const char* filename);

	/*Appends a file streamed in signed 16-bits.*/
	LIBRETTI_API void lb_appendBinaryS16ToFile(lb_BinaryS16* binary, const char* filename);

	/*Decodes an audio struct to a PCM stream.*/
	LIBRETTI_API lb_BinaryS16 lb_getSpectrumData(lb_Composition* audio);

	/*Deletes playback memory allocation.*/
	LIBRETTI_API void lb_freePlayback(lb_Playback* playback);

	/*Deletes audio memory allocation.*/
	LIBRETTI_API void lb_freeComposition(lb_Composition* composition);

	/*Deletes memory allocation of playback, note waves and audio, and the container Libretti.*/
	LIBRETTI_API void lb_freeLibretti(lb_Libretti* libretti);

	LIBRETTI_API void lb_finalize();

#ifdef __cplusplus
}
#endif

#endif /*Libretti_h*/