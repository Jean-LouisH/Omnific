/**
*                      This file is part of:
*                          Libretti
*
* Runtime
*
* Play runtime data for track position, playing, pausing, etc.
*
* Copyright (c) 2017 Jean-Louis Haywood. All rights reserved.
*/

#ifndef Playback_h
#define Playback_h

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "constants.h"
#include "waveforms.h"
#include "effects.h"

enum PlaybackStates
{
	LB_PLAYBACK_STATE_IS_PLAYING = 1 << 0,
	LB_PLAYBACK_STATE_PLAYED_AT_LEAST_ONCE = 1 << 1
};

typedef struct
{
	float current_play_time; //in seconds
	uint32_t track_note_indices[MAX_TRACKS];
	uint32_t audio_device_id;
	uint8_t play_states;
	uint16_t current_loop_count;
	lb_Waveforms current_waveforms;
	char current_lyrics[128];
	lb_Effects effects_override;
	float output_volume;
}lb_Playback;

#ifdef __cplusplus
}
#endif

#endif /*Playback_h*/