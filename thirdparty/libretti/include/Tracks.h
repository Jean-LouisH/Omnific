/**
*                      This file is part of:
*                          Libretti
*
* Tracks
*
* Individual tracks of notes that comprise audio structs.
*
* Copyright (c) 2017-2021 Jean-Louis Haywood. All rights reserved.
*/

#ifndef Tracks_h
#define Tracks_h

#include <stdint.h>
#include "Events.h"

typedef struct
{
	lb_NoteEvent* noteEvents;
	uint32_t noteCount;
}lb_Track;

#endif /*Tracks_h*/