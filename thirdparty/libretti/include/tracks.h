/**
*                      This file is part of:
*                          Libretti
*
* Tracks
*
* Individual tracks of notes that comprise audio structs.
*
* Copyright (c) 2017 Jean-Louis Haywood. All rights reserved.
*/

#ifndef Tracks_h
#define Tracks_h

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "events.h"

typedef struct
{
	lb_NoteEvent* note_events;
	uint32_t note_count;
}lb_Track;

#ifdef __cplusplus
}
#endif

#endif /*Tracks_h*/