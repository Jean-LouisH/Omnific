#include "include/Libretti.h"

#include <SDL.h>
#include "include/ScriptValidator.h"
#include "include/Compiler.h"
#include "include/Callback.h"
#include "include/File.h"
#include "include/Mixer.h"
#include "include/CallbackList.h"
#include "include/WaveformGenerator.h"

static CallbackList* g_callbackList = NULL;
static int g_librettiIDCount = -1;

void lb_initialize()
{
	if (g_callbackList == NULL)
	{
		g_callbackList = calloc(1, sizeof(CallbackList));
		if (g_callbackList != NULL)
		{
			g_librettiIDCount = 0;
			g_callbackList->capacity = 2;
			g_callbackList->librettiList = calloc(g_callbackList->capacity, sizeof(lb_Libretti*));
			initAudioPlayback(g_callbackList);
		}
		else
		{
			g_librettiIDCount = -1;
		}
	}
}

lb_Libretti* lb_createLibretti(const char* filename)
{
	lb_Libretti* libretti = malloc(sizeof *libretti);
	if (libretti != NULL)
	{
		libretti->composition = lb_createComposition(filename);
		libretti->noteWaves = lb_createNoteWaves();
		libretti->playback = lb_createPlayback();
		if (g_librettiIDCount != -1)
		{
			libretti->id = g_librettiIDCount;
			g_librettiIDCount++;
		}
	}
	return libretti;
}

lb_Libretti* lb_createEmptyLibretti()
{
	lb_Libretti* libretti = malloc(sizeof *libretti);
	if (libretti != NULL)
	{
		libretti->composition = lb_createEmptyComposition();
		libretti->noteWaves = lb_createNoteWaves();
		libretti->playback = lb_createPlayback();
		if (g_librettiIDCount != -1)
		{
			libretti->id = g_librettiIDCount;
			g_librettiIDCount++;
		}
	}
	return libretti;
}

lb_Composition* lb_createComposition(const char* filename)
{
	lb_Composition* composition = calloc(1, sizeof(lb_Composition));
	if (composition != NULL)
		lb_compileCompositionFromScriptFile(composition, filename);
	return composition;
}

lb_Composition* lb_createEmptyComposition()
{
	return calloc(1, sizeof(lb_Composition));
}

lb_NoteWaves* lb_createNoteWaves()
{
	return calloc(1, sizeof(lb_NoteWaves));
}

lb_Playback* lb_createPlayback()
{
	return calloc(1, sizeof(lb_Playback));
}

void lb_addLibrettiToCallback(lb_Libretti* libretti)
{
	if (g_callbackList != NULL)
	{
		libretti->playback->device = g_callbackList->device;
		libretti->playback->playStates = 0;
		libretti->playback->currentLoopCount = 0;
		libretti->playback->userEffectsOverride.outputVolume = 1.0;
		libretti->playback->userEffectsOverride.outputPanning = 0.0;
		lb_reset(libretti);
		lb_play(libretti);

		/* Expand the CallbackList when it gets full.*/
		if (g_callbackList->size == g_callbackList->capacity - 1)
		{
			int newCapacity = g_callbackList->capacity * 2;
			g_callbackList->librettiList = realloc(g_callbackList->librettiList, newCapacity * sizeof(lb_Libretti*));
			if (g_callbackList->librettiList != NULL)
				g_callbackList->capacity = newCapacity;
		}

		/* If the capacity expansion were successful, add the Libretti
		 * to the end of the list.*/
		if (g_callbackList->size < g_callbackList->capacity - 1)
		{
			g_callbackList->librettiList[g_callbackList->size] = libretti;
			g_callbackList->size++;
		}
	}
}

void lb_removeLibrettiFromCallback(int librettiID)
{
	if (g_callbackList != NULL)
	{
		if (g_callbackList->librettiList != NULL)
		{
			/*Simple linear search implementation since they're not intended
			 * to be much larger than 10 at a time.*/
			for (int i = 0; i < g_callbackList->size; i++)
			{
				if (g_callbackList->librettiList[i]->id == librettiID)
				{
					/* If it's not the last entry, swap and shuffle all the later entries
					 * to the front of the list.*/
					for (int j = i; j < g_callbackList->size - 1; j++)
					{
						g_callbackList->librettiList[j] = g_callbackList->librettiList[j + 1];
					}

					g_callbackList->librettiList[g_callbackList->size - 1] = NULL;
					g_callbackList->size--;
					break;
				}
			}
		}
	}
}

lb_Libretti* lb_createAndAddLibrettiToCallback(const char* filename)
{
	lb_Libretti* libretti = lb_createLibretti(filename);
	if (libretti != NULL)
		lb_addLibrettiToCallback(libretti);
	return libretti;
}

lb_Libretti* lb_createAndAddEmptyLibrettiToCallback()
{
	lb_Libretti* libretti = lb_createEmptyLibretti();
	if (libretti != NULL)
		lb_addLibrettiToCallback(libretti);
	return libretti;
}

int lb_validateScriptFile(const char* filename)
{
	char* script = loadASCIIStringFromFile(filename).data;
	int validationStatuses = validateScript(script);
	free(script);
	return validationStatuses;
}

void lb_compileCompositionFromScriptFile(lb_Composition* composition, const char* filename)
{
	char* script = loadASCIIStringFromFile(filename).data;
	if (script != NULL)
	{
		compileCompositionFromScript(composition, script);
		free(script);
	}
}

void lb_updateNoteWavesFromComposition(lb_NoteWaves* noteWaves, lb_Composition* composition, lb_Playback* playback)
{
	lb_Note* currentNotes = malloc(composition->trackCount * (sizeof *currentNotes));
	lb_updateNotesFromComposition(currentNotes, composition, playback);
	noteWaves->count = composition->trackCount;
	lb_updateNoteWavesFromNotes(noteWaves, currentNotes, composition->trackCount);
	free(currentNotes);
}

void lb_updateNotesFromComposition(lb_Note currentNotes[], lb_Composition* composition, lb_Playback* playback)
{
	for (int i = 0; i < composition->trackCount; i++)
	{
		/*Prevents note searches on lyrics audio scopes*/
		if (composition->tracks[i].noteCount > 0)
		{
			lb_NoteEvent noteEvent = composition->tracks[i].noteEvents[playback->trackNoteIndices[i]];
			lb_Note silentNote = { 0 };
			lb_NoteEvent nextNoteEvent;

			if (playback->trackNoteIndices[i] < composition->tracks[i].noteCount - 1)
			{
				nextNoteEvent = composition->tracks[i].noteEvents[playback->trackNoteIndices[i] + 1];
			}
			else
			{
				nextNoteEvent.note = silentNote;
				nextNoteEvent.startTime = composition->timeLength;
			}

			/*Find and set the current note to play*/
			while ((playback->currentPlayTime > nextNoteEvent.startTime))
			{
				if (playback->trackNoteIndices[i] < composition->tracks[i].noteCount - 1)
				{
					playback->trackNoteIndices[i]++;
					noteEvent = composition->tracks[i].noteEvents[playback->trackNoteIndices[i]];
					nextNoteEvent = composition->tracks[i].noteEvents[playback->trackNoteIndices[i] + 1];
				}
				else
				{
					nextNoteEvent.note = silentNote;
					nextNoteEvent.startTime = composition->timeLength;
				}

				/*Reset the song when the time exceeds the last time stamp.*/
				if (playback->currentPlayTime > composition->timeLength)
				{
					playback->playStates |= LB_PLAYBACK_STATE_PLAYED_AT_LEAST_ONCE;
					playback->currentLoopCount++;

					if ((playback->currentLoopCount < composition->loopCount) ||
						composition->loopCount == pow(2, (sizeof composition->loopCount) * 8) - 1)
					{
						playback->currentPlayTime = composition->loopTimestamp;

						for (int j = 0; j < composition->trackCount; j++)
						{
							playback->trackNoteIndices[j] = 0;

							/*Find and set note indices to loop time stamps.*/
							while (composition->loopTimestamp >
								composition->tracks[j].noteEvents[playback->trackNoteIndices[j]].startTime)
							{
								playback->trackNoteIndices[j]++;
							}
						}
					}
					else
					{
						playback->playStates &= ~LB_PLAYBACK_STATE_IS_PLAYING;
						SDL_PauseAudioDevice(playback->device, true);
					}
				}	
			}

			const float slurTimeRatio = 1.0;
			const float normalTimeRatio = 0.85;
			const float staccatoTimeRatio = 0.15;

			float currentTimeRatio = normalTimeRatio;

			switch (noteEvent.note.articulation)
			{
				case LB_ARTICULATION_SLUR: currentTimeRatio = slurTimeRatio; break;
				case LB_ARTICULATION_NORMAL: currentTimeRatio = normalTimeRatio; break;
				case LB_ARTICULATION_STACCATO: currentTimeRatio = staccatoTimeRatio; break;
			}

			float noteLiftTime = (noteEvent.startTime +
				((nextNoteEvent.startTime - noteEvent.startTime) * currentTimeRatio));

			if (playback->currentPlayTime > noteLiftTime)
			{
				currentNotes[i] = silentNote;
			}
			else
			{
				currentNotes[i] = composition->tracks[i].noteEvents[playback->trackNoteIndices[i]].note;
			}
		}
	}
}

void lb_updateNoteWavesFromNotes(lb_NoteWaves* noteWaves, lb_Note currentNotes[], uint8_t trackCount)
{
	generateNoteWaves(noteWaves, currentNotes);
}

void lb_incrementPlayTime(lb_Libretti* libretti, float deltaTime_s)
{
	if (libretti->playback->playStates & LB_PLAYBACK_STATE_IS_PLAYING)
		libretti->playback->currentPlayTime += deltaTime_s;
}

void lb_incrementAllPlayTimes(float deltaTime_s)
{
	if (g_callbackList != NULL)
	{
		for (int i = 0; i < g_callbackList->size; i++)
		{
			lb_Libretti* libretti = g_callbackList->librettiList[i];
			lb_incrementPlayTime(libretti, deltaTime_s);
		}
	}
}

void lb_load(lb_Libretti* libretti, const char* filename)
{
	lb_compileCompositionFromScriptFile(libretti->composition, filename);
	lb_play(libretti);
}

void lb_play(lb_Libretti* libretti)
{
	libretti->playback->playStates |= LB_PLAYBACK_STATE_IS_PLAYING;
	SDL_PauseAudioDevice(libretti->playback->device, false);
}

lb_Libretti* lb_play_note_for(
	uint16_t key,
	uint16_t dynamic,
	uint8_t panning,
	uint8_t timbre,
	uint8_t articulation,
	float duration)
{
	lb_Libretti* libretti = lb_createEmptyLibretti();
	lb_Composition* composition = libretti->composition;
	composition->trackCount = 1;
	composition->tracks[0].noteEvents = calloc(1, sizeof(lb_NoteEvent));

	if (composition->tracks[0].noteEvents != NULL)
	{
		composition->tracks[0].noteCount = 1;
		composition->tracks[0].noteEvents->startTime = 0.0;
		composition->tracks[0].noteEvents->note.dynamic = dynamic;
		composition->tracks[0].noteEvents->note.articulation = articulation;
		composition->tracks[0].noteEvents->note.key = key;
		composition->tracks[0].noteEvents->note.panning = panning;
		composition->tracks[0].noteEvents->note.timbre = timbre;
		composition->keySignature = LB_KEY_SIGNATURE_C_MAJOR;
		composition->loopCount = 0;
		composition->loopTimestamp = 0.0;
		composition->lyricsEventCount = 0;
		composition->tempoEventCount = 0;
		composition->timeLength = duration;
		composition->timeSignature[0] = 4;
		composition->timeSignature[1] = 4;
		lb_addLibrettiToCallback(libretti);
	}

	return libretti;
}

lb_Libretti* lb_play_note(
	uint16_t key,
	uint16_t dynamic,
	uint8_t panning,
	uint8_t timbre)
{
	return lb_play_note_for(
		key,
		dynamic,
		panning,
		timbre,
		LB_ARTICULATION_NORMAL,
		INFINITY);
}

lb_Libretti* lb_play_simple_note_for(
	uint16_t key,
	uint16_t dynamic,
	float duration)
{
	return lb_play_note_for(
		key,
		dynamic,
		LB_PANNING_CENTRE,
		LB_TIMBRE_SQUARE_WAVE,
		LB_ARTICULATION_NORMAL,
		duration);
}

lb_Libretti* lb_play_simple_note(
	uint16_t key,
	uint16_t dynamic)
{
	return lb_play_simple_note_for(
		key,
		dynamic,
		INFINITY);
}

void lb_pause(lb_Libretti* libretti)
{
	libretti->playback->playStates &= ~LB_PLAYBACK_STATE_IS_PLAYING;
	SDL_PauseAudioDevice(libretti->playback->device, true);
}

void lb_reset(lb_Libretti* libretti)
{
	libretti->playback->currentPlayTime = 0.0;
	for (int i = 0; i < MAX_TRACKS; i++)
		libretti->playback->trackNoteIndices[i] = 0;
}

void lb_stop(lb_Libretti* libretti)
{
	lb_reset(libretti);
	lb_pause(libretti);
}

void lb_playAll()
{
	for (int i = 0; i < g_callbackList->librettiList; i++)
		lb_play(g_callbackList->librettiList[i]);
}

void lb_pauseAll()
{
	for (int i = 0; i < g_callbackList->librettiList; i++)
		lb_pause(g_callbackList->librettiList[i]);
}

void lb_resetAll()
{
	for (int i = 0; i < g_callbackList->librettiList; i++)
		lb_reset(g_callbackList->librettiList[i]);
}

void lb_stopAll()
{
	for (int i = 0; i < g_callbackList->librettiList; i++)
		lb_stop(g_callbackList->librettiList[i]);
}

lb_BinaryS16* lb_getAudioCaptureStreamBuffer()
{
	lb_BinaryS16* binary = calloc(1, sizeof *binary);
	if (binary != NULL)
	{
		binary->size = SAMPLE_SIZE;
		binary->data = calloc(SAMPLE_SIZE, sizeof * binary->data);
		initAudioCapture(binary);
	}
	return binary;
}

void lb_saveBinaryU8ToFile(lb_BinaryU8* binary, const char* filename)
{
	saveBinaryU8ToFile(binary, (char*)filename);
}

void lb_saveBinaryS16ToFile(lb_BinaryS16* binary, const char* filename)
{
	saveBinaryS16ToFile(binary, (char*)filename);
}

void lb_appendBinaryU8ToFile(lb_BinaryU8* binary, const char* filename)
{
	appendBinaryU8ToFile(binary, (char*)filename);
}

void lb_appendBinaryS16ToFile(lb_BinaryS16* binary, const char* filename)
{
	appendBinaryS16ToFile(binary, (char*)filename);
}

lb_BinaryS16 lb_getSpectrumData(lb_Composition* composition)
{
	lb_Playback* playback = lb_createPlayback();
	lb_NoteWaves* noteWaves = lb_createNoteWaves();
	lb_BinaryS16 spectrum;

	int maxNoteCount = 0;

	/* For now, with only single track support,
	 * get the longest track, which is likely where the 
	 * melody is located. */
	for (int i = 0; i < composition->trackCount; i++)
		if (composition->tracks[i].noteCount > maxNoteCount)
			maxNoteCount = composition->tracks[i].noteCount;

	int streamLength = maxNoteCount * SAMPLE_SIZE;
	spectrum.data = calloc(streamLength, sizeof(int16_t));

	/*Building the export binary stream*/
	while (!(playback->playStates & LB_PLAYBACK_STATE_PLAYED_AT_LEAST_ONCE))
	{
		int streamPosition = 0;
		lb_updateNoteWavesFromComposition(noteWaves, composition, playback);
		for (int i = 0; i < SAMPLE_SIZE; i++)
		{
			spectrum.data[streamPosition] += noteWaves->streams[0][i];
			streamPosition++;
		}
		playback->currentPlayTime += 17.0 / 1000.0;
	}

	return spectrum;
}

void lb_freePlayback(lb_Playback* playback)
{
	SDL_CloseAudioDevice(playback->device);
	free(playback);
	playback = NULL;
}

void lb_freeNoteWaves(lb_NoteWaves* noteWaves)
{
	free(noteWaves);
	noteWaves = NULL;
}

void lb_freeComposition(lb_Composition* composition)
{
	if (composition->tracks->noteEvents != NULL)
		free(composition->tracks->noteEvents);
	if (composition->tempoEvents != NULL)
		free(composition->tempoEvents);
	if (composition->lyricsEvents != NULL)
		free(composition->lyricsEvents);
	free(composition);
	composition = NULL;
}

void lb_freeLibretti(lb_Libretti* libretti)
{
	lb_freePlayback(libretti->playback);
	lb_freeNoteWaves(libretti->noteWaves);
	lb_freeComposition(libretti->composition);
	libretti->playback = NULL;
	libretti->noteWaves = NULL;
	libretti->composition = NULL;
	free(libretti);
	libretti = NULL;
}

void lb_finalize()
{
	finalizeAudioPlayback(g_callbackList);
	free(g_callbackList);
	g_callbackList = NULL;
}