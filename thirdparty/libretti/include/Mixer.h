/**
*                      This file is part of:
*                          Libretti
*
* Mixer
*
* Applies effects to the note wave streams before sending to the audio device.
*
* Copyright (c) 2017-2019 Jean-Louis Haywood. All rights reserved.
* License: https://github.com/Jean-LouisH/Libretti/blob/master/LICENSE
*/

#ifndef Mixer_h
#define Mixer_h

#include "SDL_stdinc.h"
#include "NoteWaves.h"

/*Copies the note waves and assign audio values to the 5.1 surround channel array.*/
void interleaveNoteWavesToStream(Sint16 outputStream[], lb_NoteWaves* noteWaves);

#endif /*Mixer_h*/