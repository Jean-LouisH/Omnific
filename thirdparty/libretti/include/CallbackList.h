/**
*                      This file is part of:
*                          Libretti
*
* Callback List
*
* Contains references to all actively played Libretti structs.
*
* Copyright (c) 2017-2021 Jean-Louis Haywood. All rights reserved.
*/

#ifndef CallbackList_h
#define CallbackList_h

#include "Libretti.h"
#include <stdint.h>
#include <SDL_audio.h>

typedef struct
{
	Libretti* librettiList[MAX_LIBRETTIS];
	SDL_AudioDeviceID device;
	uint8_t size;
}CallbackList;

#endif /*CallbackList_h*/