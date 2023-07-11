/**
*                      This file is part of:
*                          Libretti
*
* Script Validator
*
* Ensures the Libretti script file is valid according to the
* language specification before it is compiled.
*
* Copyright (c) 2017 Jean-Louis Haywood. All rights reserved.
*/

#ifndef Validation_h
#define Validation_h

#include <stdbool.h>
#include <stdint.h>

enum validationStatuses
{
	LB_VALIDATION_ALL_OK										= 0,
	LB_VALIDATION_SCRIPT_FILE_NOT_LOADED						= 1,
	LB_VALIDATION_NO_TRACK_SCOPE_DETECTED						= 1 << 1,
	LB_VALIDATION_INVALID_TIME_SIGNATURE_PROVIDED				= 1 << 2,
	LB_VALIDATION_INVALID_KEY_SIGNATURE_PROVIDED				= 1 << 3,
	LB_VALIDATION_INVALID_TEMPO_PROVIDED						= 1 << 4,
	LB_VALIDATION_INVALID_LOOP_VALUE							= 1 << 5,
	LB_VALIDATION_INVALID_DYNAMICS_VALUE						= 1 << 6,
	LB_VALIDATION_INVALID_PANNING_VALUE							= 1 << 7,
	LB_VALIDATION_INVALID_TIMBRE								= 1 << 8,
	LB_VALIDATION_UNCLOSED_TRACK_SCOPE							= 1 << 9,
	LB_VALIDATION_UNEXPECTED_HEADER_NAME						= 1 << 10,
	LB_VALIDATION_INVALID_USE_OF_SYMBOL							= 1 << 11,
	LB_VALIDATION_BEATS_DO_NOT_MATCH_TIME_SIG					= 1 << 12,
	LB_VALIDATION_BAR_COUNTS_DO_NOT_MATCH						= 1 << 13,
	LB_VALIDATION_OCTAVE_SHIFTS_OUT_OF_RANGE					= 1 << 14,
	LB_VALIDATION_TRACK_SCOPE_COUNT_EXCEEDS_MAXIMUM				= 1 << 15,
	LB_VALIDATION_EXPECTED_SPACE_BETWEEN_HEADER_AND_VALUE		= 1 << 16,
	LB_VALIDATION_UNCLOSED_HEADER_TAG							= 1 << 17,
	LB_VALIDATION_UNCLOSED_TUPLET								= 1 << 18,
	LB_VALIDATION_UNCLOSED_SLUR									= 1 << 19,
	LB_VALIDATION_UNCLOSED_CRESCENDO							= 1 << 20,
	LB_VALIDATION_UNCLOSED_DIMINUENDO							= 1 << 21,
	LB_VALIDATION_EXTRA_HEADER_TAG_CLOSED_BRACKET				= 1 << 22,
	LB_VALIDATION_EXTRA_TRACK_SCOPE_CLOSED_BRACKET				= 1 << 23
};

#endif /*Validation_h*/