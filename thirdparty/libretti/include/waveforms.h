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

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "constants.h"
#include "notes.h"

typedef struct
{
	int16_t streams[MAX_TRACKS][DEFAULT_STREAM_SAMPLE_SIZE];
	lb_Note note_meta_data[MAX_TRACKS];
	uint8_t count;
}lb_Waveforms;

#ifdef __cplusplus
}
#endif

#endif /*Waveforms_h*/