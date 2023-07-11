/**
*                      This file is part of:
*                          Libretti
*
* Waveform
*
* Raw audio samples from individual notes.
*
* Copyright (c) 2017 Jean-Louis Haywood. All rights reserved.
*/

#ifndef Waveforms_h
#define Waveforms_h

#include <stdint.h>
#include "SDL_stdinc.h"
#include "Constants.h"
#include "Notes.h"

typedef struct
{
	Sint16 streams[MAX_TRACKS][SAMPLE_SIZE];
	lb_Note noteMetaData[MAX_TRACKS];
	uint8_t count;
}lb_Waveforms;

#endif /*Waveforms_h*/