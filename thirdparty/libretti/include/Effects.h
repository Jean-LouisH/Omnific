/**
*                      This file is part of:
*                          Libretti
*
* Effects
*
* Contains structs with control parameters for effects.
*
* Copyright (c) 2017-2019 Jean-Louis Haywood. All rights reserved.
* License: https://github.com/Jean-LouisH/Libretti/blob/master/LICENSE
*/

#ifndef Effects_h
#define Effects_h

typedef struct
{
	float preDelay_ms;
	float roomSize_pct;
	float damping_pct;
}ReverbControl;

typedef struct
{
	float delay_ms;
	float decay_pct;
}EchoControl;

typedef struct
{
	float level_dB;
}EQControl;

typedef struct
{
	float extent_cents;
	float rate_per_s;
}VibratoControl;

typedef struct
{
	ReverbControl reverb;
	EchoControl echo;
	EQControl eq[10]; //31Hz, 62Hz, 125Hz, 250Hz, 500Hz, 1kHz, 2kHz, 4kHz, 8kHz, 16kHz
	VibratoControl vibrato;
	uint32_t crossfading_ms;
	float pitchBlend_pct;
}lb_Effects;

#endif /*Effects_h*/