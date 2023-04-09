/**
*                      This file is part of:
*                          Libretti
*
* Events
*
* Track components with timestamps.
*
* Copyright (c) 2017-2021 Jean-Louis Haywood. All rights reserved.
*/

#ifndef Events_h
#define Events_h

#include <stdint.h>
#include "Notes.h"

typedef struct
{
	lb_Note note;
	float startTime_s;
}lb_NoteEvent;

typedef struct
{
	uint8_t tempo_bpm;
	float startTime_s;
}lb_TempoEvent;

typedef struct
{
	char lyrics[128];
	float startTime_s;
}lb_LyricsEvent;

#endif /*Events_h*/