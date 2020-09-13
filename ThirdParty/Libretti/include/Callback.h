/**
*                      This file is part of:
*                          Libretti
*
* Callback
*
* Handles the audio thread and communicates with the main thread through userdata.
*
* Copyright (c) 2017-2019 Jean-Louis Haywood. All rights reserved.
* License: https://github.com/Jean-LouisH/Libretti/blob/master/LICENSE
*/

#ifndef Callback_h
#define Callback_h

#include <SDL_stdinc.h>
#include "CallbackList.h"
#include "Binary.h"

void initAudioPlayback(CallbackList callbackList[]);
void initAudioCapture(lb_Binary_s16* binary);
void runCallbackPlay(void* userdata, Uint8* stream, int byteLength);
void runCallbackCapture(void* userdata, Uint8* stream, int byteLength);

#endif /*Callback_h*/