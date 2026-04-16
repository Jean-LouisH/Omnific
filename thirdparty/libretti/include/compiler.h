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

#ifdef __cplusplus
extern "C" {
#endif

#include "composition.h"
#include <stdint.h>

void compile_composition_from_script(lb_Composition* composition, const char* script);
void allocate_memory(lb_Composition* composition, const char* script);
void build_composition_data(lb_Composition* composition, const char* script);
void tune_by_key_signature(uint8_t key_signature, char* note_char);
void assign_frequency_from_note_char(float* frequency, uint8_t octave, char note_char);
void tune_by_accidental(float* frequency, uint8_t octave, char script_char, char note_char);

#ifdef __cplusplus
}
#endif

#endif /*Compiler_h*/