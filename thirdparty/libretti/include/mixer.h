/**
*                      This file is part of:
*                          Libretti
*
* Mixer
*
* Applies effects to the note wave streams before sending to the audio device.
*
* Copyright (c) 2017 Jean-Louis Haywood. All rights reserved.
*/

#ifndef Mixer_h
#define Mixer_h

#ifdef __cplusplus
extern "C" {
#endif

#include "playback.h"
#include <stdint.h>

/*Copies the note waves and assign audio values to the 5.1 surround channel array.*/
void interleave_waveform_to_stream(int16_t* output_stream, lb_Playback* playback, uint8_t channel_count);

#ifdef __cplusplus
}
#endif

#endif /*Mixer_h*/