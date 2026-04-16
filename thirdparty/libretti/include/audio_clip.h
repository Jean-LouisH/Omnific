/**
*                      This file is part of:
*                          Libretti
*
* Audio Clip
*
* Defines the basic data structure of audio clips.
*
* Copyright (c) 2017 Jean-Louis Haywood. All rights reserved.
*/

#ifndef Audio_Clip_h
#define Audio_Clip_h

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef struct
{
	int16_t* data;
	uint32_t sample_rate;
	uint8_t channel_count;
	uint32_t samples_per_channel;
	float playback_length;
	uint32_t bytes_per_sample;
	uint64_t size;
}lb_AudioClip;

#ifdef __cplusplus
}
#endif

#endif /*Audio_Clip_h*/