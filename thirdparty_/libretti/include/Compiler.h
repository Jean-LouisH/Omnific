/**
*                      This file is part of:
*                          Libretti
*
* Compiler
*
* Builds the audio structs from text scripts.
*
* Copyright (c) 2017-2019 Jean-Louis Haywood. All rights reserved.
* License: https://github.com/Jean-LouisH/Libretti/blob/master/LICENSE
*/

#ifndef Compiler_h
#define Compiler_h

#include "Audio.h"
#include <stdint.h>

void compileAudioFromScript(lb_Audio* audio, const char* script);
void allocateMemory(lb_Audio* audio, const char* script);
void buildAudioData(lb_Audio* audio, const char* script);
void tuneByKeySignature(uint8_t keySignature, char* noteChar);
void assignFrequencyFromNoteChar(float* frequency, uint8_t octave, char noteChar);
void tuneByAccidental(float* frequency, uint8_t octave, char scriptChar, char noteChar);

#endif /*Compiler_h*/