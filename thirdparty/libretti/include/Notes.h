/**
*                      This file is part of:
*                          Libretti
*
* Notes
*
* The smallest unit of immediate sounds and channel data.
*
* Copyright (c) 2017 Jean-Louis Haywood. All rights reserved.
*/

#ifndef Notes_h
#define Notes_h

#include <stdint.h>
#include "Binary.h"
#include "Effects.h"
#include "Constants.h"

typedef struct
{
	float dynamic;
	float key;
	int8_t panning;
	uint8_t timbre;
	uint8_t articulation;
	uint16_t cue;
	int16_t sample[SAMPLE_SIZE];
	lb_Effects effects;
}lb_Note;

enum panning
{
	LB_PANNING_REAR_LEFT = -128,
	LB_PANNING_FRONT_LEFT = -64,
	LB_PANNING_CENTRE = 0,
	LB_PANNING_FRONT_RIGHT = 64,
	LB_PANNING_REAR_RIGHT = 127
};

enum timbre
{
	LB_TIMBRE_SQUARE_WAVE,
	LB_TIMBRE_SINE_WAVE,
	LB_TIMBRE_TRIANGLE_WAVE,
	LB_TIMBRE_SAWTOOTH_WAVE,
	LB_TIMBRE_PULSE_10,
	LB_TIMBRE_PULSE_25,
	LB_TIMBRE_NOISE,
	LB_TIMBRE_SAMPLE
};

/*from Pianississimo to Fortississimo.
* Out of signed 16 bit audio amplitude values.*/
enum dynamics
{
	LB_DYNAMIC_PPP = 64,
	LB_DYNAMIC_PP = 128,
	LB_DYNAMIC_P = 256,
	LB_DYNAMIC_MP = 512,
	LB_DYNAMIC_MF = 1024,
	LB_DYNAMIC_F = 2048,
	LB_DYNAMIC_FF = 4096,
	LB_DYNAMIC_FFF = 8192,
};

/*Enumerates macros as notes according to the piano key frequencies
in Hertz (Hz) which are rounded to the nearest whole number.*/

/*R represents a musical rest.*/
enum key
{
	LB_KEY_R = 0,
	LB_KEY_A0 = 28,
	LB_KEY_Bf0 = 29,
	LB_KEY_B0 = 31,
	LB_KEY_C1 = 33,
	LB_KEY_Cs1 = 35,
	LB_KEY_D1 = 37,
	LB_KEY_Ef1 = 39,
	LB_KEY_E1 = 41,
	LB_KEY_F1 = 44,
	LB_KEY_Fs1 = 46,
	LB_KEY_G1 = 49,
	LB_KEY_Gs1 = 52,
	LB_KEY_A1 = 56,
	LB_KEY_Bf1 = 58,
	LB_KEY_B1 = 62,
	LB_KEY_C2 = 65,
	LB_KEY_Cs2 = 69,
	LB_KEY_D2 = 73,
	LB_KEY_Ef2 = 78,
	LB_KEY_E2 = 82,
	LB_KEY_F2 = 87,
	LB_KEY_Fs2 = 92,
	LB_KEY_G2 = 98,
	LB_KEY_Gs2 = 104,
	LB_KEY_A2 = 110,
	LB_KEY_Bf2 = 117,
	LB_KEY_B2 = 123,
	LB_KEY_C3 = 131,
	LB_KEY_Cs3 = 139,
	LB_KEY_D3 = 147,
	LB_KEY_Ef3 = 156,
	LB_KEY_E3 = 165,
	LB_KEY_F3 = 175,
	LB_KEY_Fs3 = 185,
	LB_KEY_G3 = 196,
	LB_KEY_Gs3 = 208,
	LB_KEY_A3 = 220,
	LB_KEY_Bf3 = 233,
	LB_KEY_B3 = 247,
	LB_KEY_C4 = 262,
	LB_KEY_Cs4 = 277,
	LB_KEY_D4 = 294,
	LB_KEY_Ef4 = 311,
	LB_KEY_E4 = 330,
	LB_KEY_F4 = 349,
	LB_KEY_Fs4 = 370,
	LB_KEY_G4 = 392,
	LB_KEY_Gs4 = 415,
	LB_KEY_A4 = 440,
	LB_KEY_Bf4 = 466,
	LB_KEY_B4 = 494,
	LB_KEY_C5 = 523,
	LB_KEY_Cs5 = 554,
	LB_KEY_D5 = 587,
	LB_KEY_Ef5 = 622,
	LB_KEY_E5 = 659,
	LB_KEY_F5 = 698,
	LB_KEY_Fs5 = 740,
	LB_KEY_G5 = 784,
	LB_KEY_Gs5 = 831,
	LB_KEY_A5 = 880,
	LB_KEY_Bf5 = 932,
	LB_KEY_B5 = 988,
	LB_KEY_C6 = 1047,
	LB_KEY_Cs6 = 1109,
	LB_KEY_D6 = 1175,
	LB_KEY_Ef6 = 1245,
	LB_KEY_E6 = 1319,
	LB_KEY_F6 = 1397,
	LB_KEY_Fs6 = 1480,
	LB_KEY_G6 = 1568,
	LB_KEY_Gs6 = 1661,
	LB_KEY_A6 = 1760,
	LB_KEY_Bf6 = 1865,
	LB_KEY_B6 = 1976,
	LB_KEY_C7 = 2093,
	LB_KEY_Cs7 = 2217,
	LB_KEY_D7 = 2349,
	LB_KEY_Ef7 = 2489,
	LB_KEY_E7 = 2637,
	LB_KEY_F7 = 2794,
	LB_KEY_Fs7 = 2960,
	LB_KEY_G7 = 3136,
	LB_KEY_Gs7 = 3322,
	LB_KEY_A7 = 3520,
	LB_KEY_Bf7 = 3729,
	LB_KEY_B7 = 3951,
	LB_KEY_C8 = 4186
};

enum articulation
{
	LB_ARTICULATION_NORMAL,
	LB_ARTICULATION_SLUR,
	LB_ARTICULATION_STACCATO
};


#endif /*Notes_h*/