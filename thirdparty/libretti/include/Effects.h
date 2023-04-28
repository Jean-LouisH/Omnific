/**
*                      This file is part of:
*                          Libretti
*
* Effects
*
* Contains structs with control parameters for effects.
*
* Copyright (c) 2017-2021 Jean-Louis Haywood. All rights reserved.
*/

#ifndef Effects_h
#define Effects_h

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
}lb_Effects;

#endif /*Effects_h*/