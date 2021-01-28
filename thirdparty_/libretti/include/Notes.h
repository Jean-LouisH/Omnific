/**
*                      This file is part of:
*                          Libretti
*
* Notes
*
* The smallest unit of immediate sounds and channel data.
*
* Copyright (c) 2017-2019 Jean-Louis Haywood. All rights reserved.
* License: https://github.com/Jean-LouisH/Libretti/blob/master/LICENSE
*/

#ifndef Notes_h
#define Notes_h

#include <stdint.h>
#include "Binary.h"
#include "Effects.h"

typedef struct
{
	float amplitude;
	float frequency_Hz;
	int8_t panning;
	uint8_t timbre;
	uint8_t articulation;
	uint16_t cue;
	lb_Binary_s16 sample;
	lb_Effects effects;
}lb_Note;

enum panning
{
	REAR_LEFT = -128,
	FRONT_LEFT = -64,
	CENTRE = 0,
	FRONT_RIGHT = 64,
	REAR_RIGHT = 127
};

enum timbre
{
	SQUARE_WAVE,
	SINE_WAVE,
	TRIANGLE_WAVE,
	SAWTOOTH_WAVE,
	PULSE_10,
	PULSE_25,
	NOISE,
	METALLIC,
	SAMPLE
};

/*from Pianississimo to Fortississimo.
* Out of signed 16 bit audio amplitude values.*/
enum dynamics
{
	PPP = 64,
	PP = 128,
	P = 256,
	MP = 512,
	MF = 1024,
	F = 2048,
	FF = 4096,
	FFF = 8192,
};

/*Enumerates macros as notes according to the piano key frequencies
in Hertz (Hz) which are rounded to the nearest whole number.*/

/*R represents a musical rest.*/
enum keyFrequencies
{
	R = 0,
	A0 = 28,
	Bf0 = 29,
	B0 = 31,
	C1 = 33,
	Cs1 = 35,
	D1 = 37,
	Ef1 = 39,
	E1 = 41,
	F1 = 44,
	Fs1 = 46,
	G1 = 49,
	Gs1 = 52,
	A1 = 56,
	Bf1 = 58,
	B1 = 62,
	C2 = 65,
	Cs2 = 69,
	D2 = 73,
	Ef2 = 78,
	E2 = 82,
	F2 = 87,
	Fs2 = 92,
	G2 = 98,
	Gs2 = 104,
	A2 = 110,
	Bf2 = 117,
	B2 = 123,
	C3 = 131,
	Cs3 = 139,
	D3 = 147,
	Ef3 = 156,
	E3 = 165,
	F3 = 175,
	Fs3 = 185,
	G3 = 196,
	Gs3 = 208,
	A3 = 220,
	Bf3 = 233,
	B3 = 247,
	C4 = 262,
	Cs4 = 277,
	D4 = 294,
	Ef4 = 311,
	E4 = 330,
	F4 = 349,
	Fs4 = 370,
	G4 = 392,
	Gs4 = 415,
	A4 = 440,
	Bf4 = 466,
	B4 = 494,
	C5 = 523,
	Cs5 = 554,
	D5 = 587,
	Ef5 = 622,
	E5 = 659,
	F5 = 698,
	Fs5 = 740,
	G5 = 784,
	Gs5 = 831,
	A5 = 880,
	Bf5 = 932,
	B5 = 988,
	C6 = 1047,
	Cs6 = 1109,
	D6 = 1175,
	Ef6 = 1245,
	E6 = 1319,
	F6 = 1397,
	Fs6 = 1480,
	G6 = 1568,
	Gs6 = 1661,
	A6 = 1760,
	Bf6 = 1865,
	B6 = 1976,
	C7 = 2093,
	Cs7 = 2217,
	D7 = 2349,
	Ef7 = 2489,
	E7 = 2637,
	F7 = 2794,
	Fs7 = 2960,
	G7 = 3136,
	Gs7 = 3322,
	A7 = 3520,
	Bf7 = 3729,
	B7 = 3951,
	C8 = 4186
};

enum articulation
{
	NORMAL,
	SLUR,
	STACCATO
};


#endif /*Notes_h*/