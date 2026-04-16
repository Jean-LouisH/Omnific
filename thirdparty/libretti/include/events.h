/**
*                      This file is part of:
*                          Libretti
*
* Events
*
* Track components with timestamps.
*
* Copyright (c) 2017 Jean-Louis Haywood. All rights reserved.
*/

#ifndef Events_h
#define Events_h

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "notes.h"
#include "constants.h"

typedef struct
{
	lb_Note note;
	float start_time; //in seconds
}lb_NoteEvent;

typedef struct
{
	uint8_t tempo; //in beats per minute
	float start_time; //in seconds
}lb_TempoEvent;

typedef struct
{
	char lyrics[LYRICS_LENGTH];
	float start_time; //in seconds
}lb_LyricsEvent;

#ifdef __cplusplus
}
#endif

#endif /*Events_h*/