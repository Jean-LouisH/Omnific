/**
*                      This file is part of:
*                          Libretti
*
* Effects
*
* Contains structs with control parameters for effects.
*
* Copyright (c) 2017 Jean-Louis Haywood. All rights reserved.
*/

#ifndef Effects_h
#define Effects_h

#include <stdint.h>

enum EffectSwitch
{
	LB_EFFECT_PAN_OUTPUT = 1 << 0,
	LB_EFFECT_REVERB_OUTPUT = 1 << 1,
	LB_EFFECT_ECHO_OUTPUT = 1 << 2,
	LB_EFFECT_EQ_OUTPUT = 1 << 3,
	LB_EFFECT_VIBRATO_OUTPUT = 1 << 4,
	LB_EFFECT_FLANGE_OUTPUT = 1 << 5,
	LB_EFFECT_CROSSFADE_OUTPUT = 1 << 6,
	LB_EFFECT_PITCH_BLEND_OUTPUT = 1 << 7,
	LB_EFFECT_TIMBRE_CHANGE_TRACK = 1 << 8,
	LB_EFFECT_FORCE_REPEAT = 1 << 9,
	LB_EFFECT_ADJUST_PLAY_SPEED = 1 << 10
};

typedef struct
{
	float preDelay; //in milliseconds
	float roomSize; //in percentages
	float damping; //in percentages
}ReverbControl;

typedef struct
{
	float delay; //in milliseconds
	float decay; //in percentages
}EchoControl;

typedef struct
{
	float level; //in decibels
}EQControl;

typedef struct
{
	float extent; //in cents
	float rate; //oscillations per second
}VibratoControl;

typedef struct
{
	ReverbControl reverb;
	EchoControl echo;
	EQControl eq[10]; //31Hz, 62Hz, 125Hz, 250Hz, 500Hz, 1kHz, 2kHz, 4kHz, 8kHz, 16kHz
	VibratoControl vibrato;
	uint32_t crossfading; //in milliseconds
	float pitchBlend; //in percentages
	uint16_t controlSwitches;
}lb_Effects;

#endif /*Effects_h*/