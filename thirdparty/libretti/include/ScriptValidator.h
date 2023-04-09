/**
*                      This file is part of:
*                          Libretti
*
* Script Validator
*
* Ensures the Libretti script file is valid according to the
* language specification before it is compiled.
*
* Copyright (c) 2017-2021 Jean-Louis Haywood. All rights reserved.
*/

#ifndef ScriptValidator_h
#define ScriptValidator_h

#include <stdbool.h>
#include <stdint.h>
#include "ScriptParseStates.h"

enum validationStatuses
{
	ALL_OK											= 0,
	NO_TRACK_SCOPE_DETECTED							= 1,
	INVALID_TIME_SIG_PROVIDED						= 1 << 1,
	INVALID_KEY_SIG_PROVIDED						= 1 << 2,
	INVALID_TEMPO_PROVIDED							= 1 << 3,
	INVALID_LOOP_VALUE								= 1 << 4,
	INVALID_DYNAMICS_VALUE							= 1 << 5,
	INVALID_PANNING_VALUE							= 1 << 6,
	INVALID_TIMBRE									= 1 << 7,
	UNCLOSED_TRACK_SCOPE							= 1 << 8,
	UNEXPECTED_HEADER_NAME							= 1 << 9,
	INVALID_USE_OF_SYMBOL							= 1 << 10,
	BEATS_DO_NOT_MATCH_TIME_SIG						= 1 << 11,
	BAR_COUNTS_DO_NOT_MATCH							= 1 << 12,
	OCTAVE_SHIFTS_OUT_OF_RANGE						= 1 << 13,
	TRACK_SCOPE_COUNT_EXCEEDS_MAXIMUM				= 1 << 14,
	EXPECTED_SPACE_BETWEEN_HEADER_AND_VALUE			= 1 << 15,
	UNCLOSED_HEADER_TAG								= 1 << 16,
	UNCLOSED_TUPLET									= 1 << 17,
	UNCLOSED_SLUR									= 1 << 18,
	UNCLOSED_CRESCENDO								= 1 << 19,
	UNCLOSED_DIMINUENDO								= 1 << 20,
	EXTRA_HEADER_TAG_CLOSED_BRACKET					= 1 << 21,
	EXTRA_TRACK_SCOPE_CLOSED_BRACKET				= 1 << 22	
};

int validateScript(const char* script);
bool validateSymbol(char symbol, uint8_t parseState);
void printCompilationStatuses(int validationStatuses);

#endif /*ScriptValidator_h*/