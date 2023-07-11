#include "include/Callback.h" 
#include "include/CallbackList.h"
#include "include/File.h"
#include "include/Mixer.h"
#include <SDL.h>

void initializeAudioPlayback(CallbackList callbackList[])
{
	if (SDL_Init(SDL_INIT_AUDIO) < 0)
	{
		SDL_Log("SDL_Init failed: %s", SDL_GetError());
	}
	else
	{
		SDL_AudioSpec desired;
		SDL_AudioSpec obtained;

		SDL_memset(&desired, 0, sizeof(desired));
		desired.freq = SAMPLE_FREQUENCY;
		desired.format = AUDIO_S16SYS;
		desired.channels = CHANNELS;
		desired.samples = SAMPLE_SIZE;
		desired.callback = (SDL_AudioCallback)runCallbackPlay;
		desired.userdata = callbackList;

		SDL_AudioDeviceID device = SDL_OpenAudioDevice(
			NULL,
			0,
			&desired,
			&obtained,
			NULL);

		SDL_PauseAudioDevice(device, 0);
		callbackList->audioDeviceID = device;
	}
}

void initializeAudioCapture(lb_BinaryS16* binary)
{
	if (SDL_Init(SDL_INIT_AUDIO) < 0)
	{
		SDL_Log("SDL_Init failed: %s", SDL_GetError());
	}
	else
	{
		SDL_AudioSpec desired;
		SDL_AudioSpec obtained;

		SDL_memset(&desired, 0, sizeof(desired));
		desired.freq = SAMPLE_FREQUENCY;
		desired.format = AUDIO_S16SYS;
		desired.channels = 1;
		desired.samples = SAMPLE_SIZE;
		desired.callback = (SDL_AudioCallback)runCallbackCapture;
		desired.userdata = binary;

		SDL_AudioDeviceID device = SDL_OpenAudioDevice(
			NULL,
			1,
			&desired,
			&obtained,
			NULL);

		SDL_PauseAudioDevice(device, 0);
	}
}

void runCallbackPlay(void* userdata, Uint8* stream, int byteLength)
{
	/*Converts the 8-bit native stream to 16-bits and references
	a double-byte length that points to the stream samples as a collection
	of two bytes.*/
	Sint16* playbackStream = (Sint16*)stream;
	int doubleByteLength = byteLength / sizeof(Sint16);
	CallbackList* callbackList = (CallbackList*)userdata;

	Sint16 debugStream[SAMPLE_SIZE * 6] = { 0 };

	/*Clears stream after accumulating channels*/
	for (int i = 0; i < doubleByteLength; i++)
	{
		playbackStream[i] = 0;
	}

	if (callbackList != NULL)
	{
		for (int i = 0; i < callbackList->size; i++)
		{
			lb_Libretti* libretti = callbackList->librettiList[i];

			if (libretti != NULL &&
				libretti->composition != NULL &&
				libretti->playback != NULL &&
				libretti->composition->trackCount > 0)
			{
				lb_updatePlayback(libretti->playback, libretti->composition);
				interleaveWaveformToStream(playbackStream, libretti->playback);
			}
		}
	}
}

void runCallbackCapture(void* userdata, Uint8* stream, int byteLength)
{
	/*Converts the 8-bit native stream to 16-bits and references
	a double-byte length that points to the stream samples as a collection
	of two bytes.*/
	Sint16* captureStream = (Sint16*)stream;
	int doubleByteLength = byteLength / sizeof(Sint16);
	lb_BinaryS16* binary = (lb_BinaryS16*)userdata;

	int16_t debug[SAMPLE_SIZE];

	for (int i = 0; i < binary->size; i++)
		binary->data[i] = captureStream[i];

	for (int i = 0; i < binary->size; i++)
		debug[i] = binary->data[i];

	//debugging
	lb_appendBinaryS16ToFile(binary, "audio_recording_dump.bin");
}

void finalizeAudioPlayback(CallbackList callbackList[])
{
	SDL_PauseAudioDevice(callbackList->audioDeviceID, 1);
	SDL_CloseAudioDevice(callbackList->audioDeviceID);
	SDL_CloseAudio();
	callbackList->audioDeviceID = 0;
}