/**
*                      This file is part of:
*                          Libretti
*
* Runtime
*
* Play runtime data for track position, playing, pausing, etc.
*
* Copyright (c) 2017-2019 Jean-Louis Haywood. All rights reserved.
* License: https://github.com/Jean-LouisH/Libretti/blob/master/LICENSE
*/

#ifndef Runtime_h
#define Runtime_h

#include <stdint.h>
#include "UserEffectsOverride.h"
#include <SDL_audio.h>

enum states
{
	IS_PLAYING = 1 << 0,
	PLAYED_AT_LEAST_ONCE = 1 << 1
};

typedef struct
{
	float currentPlayTime_s;
	uint32_t trackNoteIndices[MAX_TRACKS];
	SDL_AudioDeviceID device;
	uint8_t playStates;
	uint16_t currentLoopCount;
	lb_UserEffectsOverride userEffectsOverride;
}lb_Runtime;

#endif /*Runtime_h*/