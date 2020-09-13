/**
*                      This file is part of:
*                          Libretti
*
* User Effects Override
*
* Represents the runtime controllable settings overrided by the application.
*
* Copyright (c) 2017-2019 Jean-Louis Haywood. All rights reserved.
* License: https://github.com/Jean-LouisH/Libretti/blob/master/LICENSE
*/

#ifndef UserEffectsOverride_h
#define UserEffectsOverride_h

#include <stdint.h>
#include "Effects.h"

enum userEffectsOverrideCodes
{
	PAN_OUTPUT				= 1 << 0,
	REVERB_OUTPUT			= 1 << 1,
	ECHO_OUTPUT				= 1 << 2,
	EQ_OUTPUT				= 1 << 3,
	VIBRATO_OUTPUT			= 1 << 4,
	FLANGE_OUTPUT			= 1 << 5,
	CROSSFADE_OUTPUT		= 1 << 6,
	PITCH_BLEND_OUTPUT		= 1 << 7,
	TIMBRE_CHANGE_TRACK		= 1 << 8,
	FORCE_REPEAT			= 1 << 9,
	ADJUST_PLAY_SPEED		= 1 << 10
};

typedef struct
{
	float outputVolume;
	float outputPanning; //-1.0 to 1.0
	lb_Effects effects;
	uint32_t controlSwitches;
}lb_UserEffectsOverride;

#endif /*UserEffectsOverride_h*/