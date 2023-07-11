/**
*                      This file is part of:
*                          Libretti
*
* Compiler
*
* Builds the audio structs from text scripts.
*
* Copyright (c) 2017 Jean-Louis Haywood. All rights reserved.
*/

#ifndef Compiler_h
#define Compiler_h

#include "Composition.h"
#include <stdint.h>

void compileCompositionFromScript(lb_Composition* composition, const char* script);
void allocateMemory(lb_Composition* composition, const char* script);
void buildCompositionData(lb_Composition* composition, const char* script);
void tuneByKeySignature(uint8_t keySignature, char* noteChar);
void assignFrequencyFromNoteChar(float* frequency, uint8_t octave, char noteChar);
void tuneByAccidental(float* frequency, uint8_t octave, char scriptChar, char noteChar);

#endif /*Compiler_h*/