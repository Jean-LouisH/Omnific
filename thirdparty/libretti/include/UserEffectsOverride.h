/**
*                      This file is part of:
*                          Libretti
*
* User Effects Override
*
* Represents the runtime controllable settings overrided by the application.
*
* Copyright (c) 2017-2021 Jean-Louis Haywood. All rights reserved.
*/

#ifndef UserEffectsOverride_h
#define UserEffectsOverride_h

#include <stdint.h>
#include "Effects.h"

enum userEffectsOverrideCodes
{
	LB_EFFECT_PAN_OUTPUT				= 1 << 0,
	LB_EFFECT_REVERB_OUTPUT				= 1 << 1,
	LB_EFFECT_ECHO_OUTPUT				= 1 << 2,
	LB_EFFECT_EQ_OUTPUT					= 1 << 3,
	LB_EFFECT_VIBRATO_OUTPUT			= 1 << 4,
	LB_EFFECT_FLANGE_OUTPUT				= 1 << 5,
	LB_EFFECT_CROSSFADE_OUTPUT			= 1 << 6,
	LB_EFFECT_PITCH_BLEND_OUTPUT		= 1 << 7,
	LB_EFFECT_TIMBRE_CHANGE_TRACK		= 1 << 8,
	LB_EFFECT_FORCE_REPEAT				= 1 << 9,
	LB_EFFECT_ADJUST_PLAY_SPEED			= 1 << 10
};

typedef struct
{
	float outputVolume;
	float outputPanning; //-1.0 to 1.0
	lb_Effects effects;
	uint32_t controlSwitches;
}lb_UserEffectsOverride;

#endif /*UserEffectsOverride_h*/