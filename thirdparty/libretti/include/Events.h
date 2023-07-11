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

#include <stdint.h>
#include "Notes.h"
#include "Constants.h"

typedef struct
{
	lb_Note note;
	float startTime; //in seconds
}lb_NoteEvent;

typedef struct
{
	uint8_t tempo; //in beats per minute
	float startTime; //in seconds
}lb_TempoEvent;

typedef struct
{
	char lyrics[LYRICS_LENGTH];
	float startTime; //in seconds
}lb_LyricsEvent;

#endif /*Events_h*/