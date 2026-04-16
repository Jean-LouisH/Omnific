/**
*                      This file is part of:
*                          Libretti
*
* Audio
*
* Defines entire songs or sound effects in memory.
*
* Copyright (c) 2017 Jean-Louis Haywood. All rights reserved.
*/

#ifndef Composition_h
#define Composition_h

#ifdef __cplusplus
extern "C" {
#endif

#include "constants.h"
#include "events.h"
#include "tracks.h"
#include <stdint.h>

typedef struct
{
	lb_Track tracks[MAX_TRACKS];
	lb_TempoEvent* tempo_events;
	lb_LyricsEvent* lyrics_events;
	uint8_t track_count;
	uint8_t tempo_event_count;
	uint8_t lyrics_event_count;
	float time_length;				//in seconds
	float loop_timestamp;			//in seconds
	uint16_t loop_count;
	char name[64];
	char artist[32];
	uint8_t key_signature;
	uint8_t time_signature[2]; //[0] = numerator, [1] = denominator
	int validation_statuses;
}lb_Composition;

#ifdef __cplusplus
}
#endif

#endif /*Composition_h*/