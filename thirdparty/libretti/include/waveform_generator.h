/**
*                      This file is part of:
*                          Libretti
*
* Waveform Generator
*
* Provides note waves from notes.
*
* Copyright (c) 2017 Jean-Louis Haywood. All rights reserved.
*/

#ifndef WaveformGenerator_h
#define WaveformGenerator_h

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "notes.h"
#include "waveforms.h"

void generate_waveform(lb_Waveforms* waveforms, lb_Note current_notes[], int sample_rate);

#ifdef __cplusplus
}
#endif

#endif /*WaveformGenerator_h*/