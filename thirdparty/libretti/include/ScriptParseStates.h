/**
*                      This file is part of:
*                          Libretti
*
* Script Parse States
*
* For all script parsers to determine the correct segments of
* Libretti scripts according to the language specification.
*
* Copyright (c) 2017-2021 Jean-Louis Haywood. All rights reserved.
*/

#ifndef ScriptParseStates_h
#define ScriptParseStates_h

enum parseStates
{
	READING_NOTHING,
	READING_HEADER,
	IGNORING_FIRST_SPACE_IN_VALUE,
	READING_VALUE,
	READING_TRACK_SCOPE,
	READING_NOTE_FREQUENCY,
	READING_NOTE_ACCIDENTAL,
	READING_NOTE_DURATION,
	READING_DYNAMIC_ACCENT,
	READING_STACCATO
};

#endif /*ScriptParseStates_h*/