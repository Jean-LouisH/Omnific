/**
*                      This file is part of:
*                          Libretti
*
* Audio
*
* Defines entire songs or sound effects in memory.
*
* Copyright (c) 2017-2019 Jean-Louis Haywood. All rights reserved.
* License: https://github.com/Jean-LouisH/Libretti/blob/master/LICENSE
*/

#ifndef Audio_h
#define Audio_h

#include "Constants.h"
#include "Events.h"
#include "Tracks.h"
#include <stdint.h>

typedef struct
{
	lb_Track tracks[MAX_TRACKS];
	lb_TempoEvent* tempoEvents;
	lb_LyricsEvent* lyricsEvents;
	uint8_t trackCount;
	uint8_t tempoEventCount;
	uint8_t lyricsEventCount;
	float timeLength_s;
	float loopTimestamp_s;
	uint16_t loopCount;
	char name[64];
	char artist[32];
	uint8_t keySignature;
	uint8_t timeSignature[2]; //[0] = numerator, [1] = denominator
}lb_Audio;

#endif /*Audio_h*/