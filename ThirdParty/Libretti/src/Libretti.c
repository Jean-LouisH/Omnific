#include "Libretti.h"

#include <SDL.h>
#include "ScriptValidator.h"
#include "Compiler.h"
#include "Callback.h"
#include "File.h"
#include "Mixer.h"
#include "CallbackList.h"
#include "WaveformGenerator.h"

static CallbackList* callbackList;

Libretti* lb_createLibretti(const char* filename)
{
	Libretti* libretti = malloc(sizeof *libretti);
	if (libretti != NULL)
	{
		libretti->audio = lb_createAudio(filename);
		libretti->noteWaves = lb_createNoteWaves();
		libretti->runtime = lb_createRuntime();
	}
	return libretti;
}

Libretti* lb_createEmptyLibretti()
{
	Libretti* libretti = malloc(sizeof *libretti);
	if (libretti != NULL)
	{
		libretti->audio = lb_createEmptyAudio();
		libretti->noteWaves = lb_createNoteWaves();
		libretti->runtime = lb_createRuntime();
	}
	return libretti;
}

lb_Audio* lb_createAudio(const char* filename)
{
	lb_Audio* audio = calloc(1, sizeof(lb_Audio));
	if (audio != NULL)
		lb_compileAudioFromScriptFile(audio, filename);
	return audio;
}

lb_Audio* lb_createEmptyAudio()
{
	return calloc(1, sizeof(lb_Audio));
}

lb_NoteWaves* lb_createNoteWaves()
{
	return calloc(1, sizeof(lb_NoteWaves));
}

lb_Runtime* lb_createRuntime()
{
	return calloc(1, sizeof(lb_Runtime));
}

void lb_addLibrettiToCallback(Libretti* libretti)
{
	if (callbackList == NULL)
	{
		callbackList = calloc(1, sizeof(CallbackList));
		initAudioPlayback(callbackList);
	}
	
	//Checks for allocation after calloc()
	if (callbackList != NULL)
	{
		libretti->runtime->device = callbackList->device;
		libretti->runtime->playStates = 0;
		libretti->runtime->currentLoopCount = 0;
		libretti->runtime->userEffectsOverride.outputVolume = 1.0;
		libretti->runtime->userEffectsOverride.outputPanning = 0.0;
		lb_reset(libretti);
		lb_play(libretti);
		callbackList->librettiList[callbackList->size] = libretti;
		callbackList->size++;
	}
}

Libretti* lb_createAndAddLibrettiToCallback(const char* filename)
{
	Libretti* libretti = lb_createLibretti(filename);
	if (libretti != NULL)
		lb_addLibrettiToCallback(libretti);
	return libretti;
}

Libretti* lb_createAndAddEmptyLibrettiToCallback()
{
	Libretti* libretti = lb_createEmptyLibretti();
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

void lb_compileAudioFromScriptFile(lb_Audio* audio, const char* filename)
{
	char* script = loadASCIIStringFromFile(filename).data;
	if (script != NULL)
	{
		compileAudioFromScript(audio, script);
		free(script);
	}
}

void lb_updateNoteWavesFromAudio(lb_NoteWaves* noteWaves, lb_Audio* audio, lb_Runtime* runtime)
{
	lb_Note* currentNotes = malloc(audio->trackCount * (sizeof *currentNotes));
	lb_updateNotesFromAudio(currentNotes, audio, runtime);
	noteWaves->count = audio->trackCount;
	lb_updateNoteWavesFromNotes(noteWaves, currentNotes, audio->trackCount);
	free(currentNotes);
}

void lb_updateNotesFromAudio(lb_Note currentNotes[], lb_Audio* audio, lb_Runtime* runtime)
{
	for (int i = 0; i < audio->trackCount; i++)
	{
		/*Prevents note searches on lyrics audio scopes*/
		if (audio->tracks[i].noteCount > 0)
		{
			lb_NoteEvent noteEvent = audio->tracks[i].noteEvents[runtime->trackNoteIndices[i]];
			lb_Note silentNote = { 0 };
			lb_NoteEvent nextNoteEvent;

			if (runtime->trackNoteIndices[i] < audio->tracks[i].noteCount - 1)
			{
				nextNoteEvent = audio->tracks[i].noteEvents[runtime->trackNoteIndices[i] + 1];
			}
			else
			{
				nextNoteEvent.note = silentNote;
				nextNoteEvent.startTime_s = audio->timeLength_s;
			}

			/*Find and set the current note to play*/
			while ((runtime->currentPlayTime_s > nextNoteEvent.startTime_s))
			{
				if (runtime->trackNoteIndices[i] < audio->tracks[i].noteCount - 1)
				{
					runtime->trackNoteIndices[i]++;
					noteEvent = audio->tracks[i].noteEvents[runtime->trackNoteIndices[i]];
					nextNoteEvent = audio->tracks[i].noteEvents[runtime->trackNoteIndices[i] + 1];
				}
				else
				{
					nextNoteEvent.note = silentNote;
					nextNoteEvent.startTime_s = audio->timeLength_s;
				}

				/*Reset the song when the time exceeds the last time stamp.*/
				if (runtime->currentPlayTime_s > audio->timeLength_s)
				{
					runtime->playStates |= PLAYED_AT_LEAST_ONCE;
					runtime->currentLoopCount++;

					if ((runtime->currentLoopCount < audio->loopCount) ||
						audio->loopCount == pow(2, (sizeof audio->loopCount) * 8) - 1)
					{
						runtime->currentPlayTime_s = audio->loopTimestamp_s;

						for (int j = 0; j < audio->trackCount; j++)
						{
							runtime->trackNoteIndices[j] = 0;

							/*Find and set note indices to loop time stamps.*/
							while (audio->loopTimestamp_s > 
								audio->tracks[j].noteEvents[runtime->trackNoteIndices[j]].startTime_s)
							{
								runtime->trackNoteIndices[j]++;
							}
						}
					}
					else
					{
						runtime->playStates &= ~IS_PLAYING;
						SDL_PauseAudioDevice(runtime->device, true);
					}
				}	
			}

			const float slurTimeRatio = 1.0;
			const float normalTimeRatio = 0.85;
			const float staccatoTimeRatio = 0.15;

			float currentTimeRatio = normalTimeRatio;

			switch (noteEvent.note.articulation)
			{
				case SLUR: currentTimeRatio = slurTimeRatio; break;
				case NORMAL: currentTimeRatio = normalTimeRatio; break;
				case STACCATO: currentTimeRatio = staccatoTimeRatio; break;
			}

			float noteLiftTime = (noteEvent.startTime_s +
				((nextNoteEvent.startTime_s - noteEvent.startTime_s) * currentTimeRatio));

			if (runtime->currentPlayTime_s > noteLiftTime)
			{
				currentNotes[i] = silentNote;
			}
			else
			{
				currentNotes[i] = audio->tracks[i].noteEvents[runtime->trackNoteIndices[i]].note;
			}
		}
	}
}

void lb_updateNoteWavesFromNotes(lb_NoteWaves* noteWaves, lb_Note currentNotes[], uint8_t trackCount)
{
	generateNoteWaves(noteWaves, currentNotes);
}

void lb_incrementPlayTime(Libretti* libretti, float deltaTime_s)
{
	if (libretti->runtime->playStates & IS_PLAYING)
		libretti->runtime->currentPlayTime_s += deltaTime_s;
}

void lb_incrementAllPlayTimes(float deltaTime_s)
{
	for (int i = 0; i < callbackList->size; i++)
	{
		Libretti* libretti = callbackList->librettiList[i];
		lb_incrementPlayTime(libretti, deltaTime_s);
	}
}

void lb_load(Libretti* libretti, const char* filename)
{
	lb_compileAudioFromScriptFile(libretti->audio, filename);
	lb_play(libretti);
}

void lb_play(Libretti* libretti)
{
	libretti->runtime->playStates |= IS_PLAYING;
	SDL_PauseAudioDevice(libretti->runtime->device, false);
}

void lb_pause(Libretti* libretti)
{
	libretti->runtime->playStates &= ~IS_PLAYING;
	SDL_PauseAudioDevice(libretti->runtime->device, true);
}

void lb_reset(Libretti* libretti)
{
	libretti->runtime->currentPlayTime_s = 0.0;
	for (int i = 0; i < MAX_TRACKS; i++)
		libretti->runtime->trackNoteIndices[i] = 0;
}

void lb_stop(Libretti* libretti)
{
	lb_reset(libretti);
	lb_pause(libretti);
}

void lb_playAll()
{
	for (int i = 0; i < callbackList->librettiList; i++)
		lb_play(callbackList->librettiList[i]);
}

void lb_pauseAll()
{
	for (int i = 0; i < callbackList->librettiList; i++)
		lb_pause(callbackList->librettiList[i]);
}

void lb_resetAll()
{
	for (int i = 0; i < callbackList->librettiList; i++)
		lb_reset(callbackList->librettiList[i]);
}

void lb_stopAll()
{
	for (int i = 0; i < callbackList->librettiList; i++)
		lb_stop(callbackList->librettiList[i]);
}

lb_Binary_s16* lb_getAudioCaptureStreamBuffer()
{
	lb_Binary_s16* binary = calloc(1, sizeof *binary);
	if (binary != NULL)
	{
		binary->size = SAMPLE_SIZE;
		binary->data = calloc(SAMPLE_SIZE, sizeof * binary->data);
		initAudioCapture(binary);
	}
	return binary;
}

void lb_saveBinaryU8ToFile(lb_Binary_u8* binary, const char* filename)
{
	saveBinaryU8ToFile(binary, (char*)filename);
}

void lb_saveBinaryS16ToFile(lb_Binary_s16* binary, const char* filename)
{
	saveBinaryS16ToFile(binary, (char*)filename);
}

void lb_appendBinaryU8ToFile(lb_Binary_u8* binary, const char* filename)
{
	appendBinaryU8ToFile(binary, (char*)filename);
}

void lb_appendBinaryS16ToFile(lb_Binary_s16* binary, const char* filename)
{
	appendBinaryS16ToFile(binary, (char*)filename);
}

void lb_exportAudioToWAV(lb_Audio* audio, const char* name)
{
	lb_Runtime* exportRuntime = lb_createRuntime();
	lb_NoteWaves* exportNoteWaves = lb_createNoteWaves();
	lb_Binary_s16 exportBinary;

	int maxNoteCount = 0;

	for (int i = 0; i < audio->trackCount; i++)
		if (audio->tracks[i].noteCount > maxNoteCount)
			maxNoteCount = audio->tracks[i].noteCount;

	int streamLength = maxNoteCount * SAMPLE_SIZE;
	exportBinary.data = calloc(streamLength, sizeof(int16_t));

	/*Building the export binary stream*/
	while (!(exportRuntime->playStates & PLAYED_AT_LEAST_ONCE))
	{
		int streamPosition = 0;
		lb_updateNoteWavesFromAudio(exportNoteWaves, audio, exportRuntime);
		for (int i = 0; i < SAMPLE_SIZE; i++)
		{
			exportBinary.data[streamPosition] += exportNoteWaves->streams[0][i];
			streamPosition++;
		}
		exportRuntime->currentPlayTime_s += 17.0 / 1000.0;
	}


	/*Exporting to WAV file*/

}

void lb_freeRuntime(lb_Runtime* runtime)
{
	SDL_CloseAudioDevice(runtime->device);
	free(runtime);
}

void lb_freeNoteWaves(lb_NoteWaves* noteWaves)
{
	free(noteWaves);
}

void lb_freeAudio(lb_Audio* audio)
{
	if (audio->tracks->noteEvents != NULL)
		free(audio->tracks->noteEvents);
	if (audio->tempoEvents != NULL)
		free(audio->tempoEvents);
	if (audio->lyricsEvents != NULL)
		free(audio->lyricsEvents);
	free(audio);
}

void lb_freeLibretti(Libretti* libretti)
{
	lb_freeRuntime(libretti->runtime);
	lb_freeNoteWaves(libretti->noteWaves);
	lb_freeAudio(libretti->audio);
	libretti->runtime = NULL;
	libretti->noteWaves = NULL;
	libretti->audio = NULL;
	free(libretti);
}