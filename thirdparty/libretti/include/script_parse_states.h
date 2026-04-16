/**
*                      This file is part of:
*                          Libretti
*
* Script Parse States
*
* For all script parsers to determine the correct segments of
* Libretti scripts according to the language specification.
*
* Copyright (c) 2017 Jean-Louis Haywood. All rights reserved.
*/

#ifndef ScriptParseStates_h
#define ScriptParseStates_h

#ifdef __cplusplus
extern "C" {
#endif

enum parse_states
{
	LB_PARSE_STATE_READING_NOTHING,
	LB_PARSE_STATE_READING_HEADER,
	LB_PARSE_STATE_IGNORING_FIRST_SPACE_IN_VALUE,
	LB_PARSE_STATE_READING_VALUE,
	LB_PARSE_STATE_READING_TRACK_SCOPE,
	LB_PARSE_STATE_READING_NOTE_FREQUENCY,
	LB_PARSE_STATE_READING_NOTE_ACCIDENTAL,
	LB_PARSE_STATE_READING_NOTE_DURATION,
	LB_PARSE_STATE_READING_DYNAMIC_ACCENT,
	LB_PARSE_STATE_READING_STACCATO
};

#ifdef __cplusplus
}
#endif

#endif /*ScriptParseStates_h*/