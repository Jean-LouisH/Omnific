/**
*                      This file is part of:
*                          Libretti
*
* Constants
*
* Copyright (c) 2017-2021 Jean-Louis Haywood. All rights reserved.
*/

#ifndef Contants_h
#define Contants_h

#define MAX_TRACKS 24
#define MAX_LIBRETTIS 16

#define CHANNELS 6	//front-left, front-right, center, low-freq, rear-left, rear-right
#define MAX_FPS 60
#define SAMPLE_FREQUENCY 44100
#define SAMPLES_PER_FRAME (SAMPLE_FREQUENCY / MAX_FPS)
#define SAMPLE_SIZE (SAMPLES_PER_FRAME > 1024 ? 2048 : 1024) //Ensures that the buffer has more than enough spaced reserved.

enum key_signatures
{
	C_MAJOR,
	G_MAJOR,
	D_MAJOR,
	A_MAJOR,
	E_MAJOR,
	B_MAJOR,
	Fs_MAJOR,
	Gb_MAJOR,
	Db_MAJOR,
	Ab_MAJOR,
	Eb_MAJOR,
	Bb_MAJOR,
	F_MAJOR,

	A_MINOR,
	E_MINOR,
	B_MINOR,
	Fs_MINOR,
	Cs_MINOR,
	Gs_MINOR,
	Ds_MINOR,
	Eb_MINOR,
	Bb_MINOR,
	F_MINOR,
	C_MINOR,
	G_MINOR,
	D_MINOR
};

#endif /*Contants_h*/	