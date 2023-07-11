/**
*                      This file is part of:
*                          Libretti
*
* Callback
*
* Handles the audio thread and communicates with the main thread through userdata.
*
* Copyright (c) 2017 Jean-Louis Haywood. All rights reserved.
*/

#ifndef Callback_h
#define Callback_h

#include <SDL_stdinc.h>
#include "CallbackList.h"
#include "Binary.h"

void initializeAudioPlayback(CallbackList callbackList[]);
void initializeAudioCapture(lb_BinaryS16* binary);
void runCallbackPlay(void* userdata, Uint8* stream, int byteLength);
void runCallbackCapture(void* userdata, Uint8* stream, int byteLength);
void finalizeAudioPlayback(CallbackList callbackList[]);

#endif /*Callback_h*/