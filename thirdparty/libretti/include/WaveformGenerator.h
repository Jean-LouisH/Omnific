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

#include <stdint.h>
#include "Notes.h"
#include "Waveforms.h"

void generateWaveform(lb_Waveforms* waveforms, lb_Note currentNotes[]);

#endif /*WaveformGenerator_h*/