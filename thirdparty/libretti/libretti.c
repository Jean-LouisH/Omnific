#include "libretti.h"

#include <math.h>
#include "playback.h"
#include "script_validator.h"
#include "validation.h"
#include "compiler.h"
#include "file.h"
#include "waveform_generator.h"
#include "mixer.h"

#include <stdlib.h>
#include <stdio.h>

#define DR_WAV_IMPLEMENTATION
#include "dr_wav.h"

static int g_libretti_id_count = -1;

lb_Libretti* lb_create_libretti(const char* filename)
{
	lb_Libretti* libretti = malloc(sizeof *libretti);
	if (libretti != 0)
	{
		libretti->composition = lb_create_composition(filename);
		libretti->playback = lb_create_playback();
		if (g_libretti_id_count != -1)
		{
			libretti->id = g_libretti_id_count;
			g_libretti_id_count++;
		}
	}
	return libretti;
}

lb_Libretti* lb_create_empty_libretti()
{
	lb_Libretti* libretti = malloc(sizeof *libretti);
	if (libretti != 0)
	{
		libretti->composition = lb_create_empty_composition();
		libretti->playback = lb_create_playback();
		if (g_libretti_id_count != -1)
		{
			libretti->id = g_libretti_id_count;
			g_libretti_id_count++;
		}
	}
	return libretti;
}

lb_Composition* lb_create_composition(const char* filename)
{
	lb_Composition* composition = lb_create_empty_composition();
	if (composition != 0)
		lb_compile_composition_from_script_file(composition, filename);
	return composition;
}

lb_Composition* lb_create_empty_composition()
{
	lb_Composition* composition = calloc(1, sizeof(lb_Composition));
	if (composition != 0)
		composition->validation_statuses = LB_VALIDATION_SCRIPT_FILE_NOT_LOADED;
	return composition;
}

lb_Playback* lb_create_playback()
{
	lb_Playback* playback = calloc(1, sizeof(lb_Playback));
	playback->output_volume = 1.0;
	return playback;
}

int lb_validate_script_file(const char* filename)
{
	char* script = load_ascii_string_from_file(filename).data;
	int validation_statuses = validate_script(script);
	free(script);
	return validation_statuses;
}

void lb_compile_composition_from_script_file(lb_Composition* composition, const char* filename)
{
	char* script = load_ascii_string_from_file(filename).data;
	if (script != 0)
	{
		compile_composition_from_script(composition, script);
		free(script);
	}
	else
	{
		composition->validation_statuses = LB_VALIDATION_SCRIPT_FILE_NOT_LOADED;
		printf("\n\n_error 0x%X: \tLB_VALIDATION_SCRIPT_FILE_NOT_LOADED -> The file '%s' may be missing.\n",
			LB_VALIDATION_SCRIPT_FILE_NOT_LOADED,
			filename);
	}
}

void lb_update_playback(lb_Playback* playback, lb_Composition* composition, int sample_rate)
{
	lb_Note* current_notes = malloc(composition->track_count * (sizeof * current_notes));
	lb_update_notes_from_composition(current_notes, composition, playback);
	playback->current_waveforms.count = composition->track_count;
	lb_update_waveform_from_notes(&playback->current_waveforms, current_notes, sample_rate);

	for (int i = 0; i < LYRICS_LENGTH; i++)
		playback->current_lyrics[i] = '\0';

	/* Find the current lyrics. */
	for (int i = 0; i < composition->lyrics_event_count; i++)
		if (playback->current_play_time > composition->lyrics_events[i].start_time)
			for (int j = 0; j < LYRICS_LENGTH; j++)
				playback->current_lyrics[j] = composition->lyrics_events[i].lyrics[j];

	free(current_notes);
}

void lb_update_notes_from_composition(lb_Note current_notes[], lb_Composition* composition, lb_Playback* playback)
{
	for (int i = 0; i < composition->track_count; i++)
	{
		/*Prevents note searches on lyrics audio scopes*/
		if (composition->tracks[i].note_count > 0)
		{
			lb_NoteEvent note_event = composition->tracks[i].note_events[playback->track_note_indices[i]];
			lb_Note silent_note = { 0 };
			lb_NoteEvent next_note_event;

			if (playback->track_note_indices[i] < composition->tracks[i].note_count - 1)
			{
				next_note_event = composition->tracks[i].note_events[playback->track_note_indices[i] + 1];
			}
			else
			{
				next_note_event.note = silent_note;
				next_note_event.start_time = composition->time_length;
			}

			/*Find and set the current note to play*/
			while ((playback->current_play_time > next_note_event.start_time))
			{
				if (playback->track_note_indices[i] < composition->tracks[i].note_count - 1)
				{
					playback->track_note_indices[i]++;
					note_event = composition->tracks[i].note_events[playback->track_note_indices[i]];
					next_note_event = composition->tracks[i].note_events[playback->track_note_indices[i] + 1];
				}
				else
				{
					next_note_event.note = silent_note;
					next_note_event.start_time = composition->time_length;
				}

				/*Reset the song when the time exceeds the last time stamp.*/
				if (playback->current_play_time > composition->time_length)
				{
					playback->play_states |= LB_PLAYBACK_STATE_PLAYED_AT_LEAST_ONCE;
					playback->current_loop_count++;

					if ((playback->current_loop_count < composition->loop_count) ||
						composition->loop_count == pow(2, (sizeof composition->loop_count) * 8) - 1)
					{
						playback->current_play_time = composition->loop_timestamp;

						for (int j = 0; j < composition->track_count; j++)
						{
							playback->track_note_indices[j] = 0;

							/*Find and set note indices to loop time stamps.*/
							while (composition->loop_timestamp >
								composition->tracks[j].note_events[playback->track_note_indices[j]].start_time)
							{
								playback->track_note_indices[j]++;
							}
						}
					}
					else
					{
						playback->play_states &= ~LB_PLAYBACK_STATE_IS_PLAYING;
					}
				}	
			}

			const float slur_time_ratio = 1.0;
			const float normal_time_ratio = 0.85;
			const float staccato_time_ratio = 0.15;

			float current_time_ratio = normal_time_ratio;

			switch (note_event.note.articulation)
			{
				case LB_ARTICULATION_SLUR: current_time_ratio = slur_time_ratio; break;
				case LB_ARTICULATION_NORMAL: current_time_ratio = normal_time_ratio; break;
				case LB_ARTICULATION_STACCATO: current_time_ratio = staccato_time_ratio; break;
			}

			float note_lift_time = (note_event.start_time +
				((next_note_event.start_time - note_event.start_time) * current_time_ratio));

			if (playback->current_play_time > note_lift_time)
			{
				current_notes[i] = silent_note;
			}
			else
			{
				current_notes[i] = composition->tracks[i].note_events[playback->track_note_indices[i]].note;
			}
		}
	}
}

void lb_update_waveform_from_notes(lb_Waveforms* waveforms, lb_Note current_notes[], int sample_rate)
{
	generate_waveform(waveforms, current_notes, sample_rate);
}

void lb_increment_play_time(lb_Libretti* libretti, float delta_time)
{
	if (libretti->playback->play_states & LB_PLAYBACK_STATE_IS_PLAYING)
		libretti->playback->current_play_time += delta_time;
}

void lb_load(lb_Libretti* libretti, const char* filename)
{
	lb_compile_composition_from_script_file(libretti->composition, filename);
	lb_play(libretti);
}

void lb_play(lb_Libretti* libretti)
{
	libretti->playback->play_states |= LB_PLAYBACK_STATE_IS_PLAYING;
}

void lb_pause(lb_Libretti* libretti)
{
	libretti->playback->play_states &= ~LB_PLAYBACK_STATE_IS_PLAYING;
}

void lb_reset(lb_Libretti* libretti)
{
	libretti->playback->current_play_time = 0.0;
	for (int i = 0; i < MAX_TRACKS; i++)
		libretti->playback->track_note_indices[i] = 0;
}

void lb_stop(lb_Libretti* libretti)
{
	lb_reset(libretti);
	lb_pause(libretti);
}

void lb_save_binary_u8_to_file(lb_BinaryU8* binary, const char* filename)
{
	save_binary_u8_to_file(binary, (char*)filename);
}

void lb_save_binary_s16_to_file(lb_BinaryS16* binary, const char* filename)
{
	save_binary_s16_to_file(binary, (char*)filename);
}

void lb_append_binary_u8_to_file(lb_BinaryU8* binary, const char* filename)
{
	append_binary_u8_to_file(binary, (char*)filename);
}

void lb_append_binary_s16_to_file(lb_BinaryS16* binary, const char* filename)
{
	append_binary_s16_to_file(binary, (char*)filename);
}

lb_AudioClip lb_synthesize_audio_clip(lb_Libretti* libretti, uint32_t sample_rate, uint8_t channel_count)
{
	lb_AudioClip audio_clip = { 0 };

	if (libretti != 0 &&
		libretti->composition != 0 &&
		libretti->playback != 0 &&
		libretti->composition->track_count > 0)
	{
		audio_clip.sample_rate = sample_rate;
		audio_clip.channel_count = channel_count;
		audio_clip.bytes_per_sample = sizeof(int16_t);
		audio_clip.samples_per_channel = (uint32_t)(libretti->composition->time_length * sample_rate);
		audio_clip.playback_length = libretti->composition->time_length;

		uint32_t total_samples = audio_clip.samples_per_channel * audio_clip.channel_count;
		audio_clip.size = total_samples * sizeof(int16_t);
		audio_clip.data = (int16_t*)calloc(total_samples, sizeof(int16_t));

		if (!audio_clip.data)
			return audio_clip;

		int stream_length = DEFAULT_STREAM_SAMPLE_SIZE * channel_count;
		int16_t* output_stream = (int16_t*)calloc(stream_length, sizeof(int16_t));

		uint32_t total_samples_written = 0;

		/*Building the export binary stream*/
		while (!(libretti->playback->play_states & LB_PLAYBACK_STATE_PLAYED_AT_LEAST_ONCE))
		{
			for (int i = 0; i < stream_length; i++)
				output_stream[i] = 0;

			int audio_clip_stream_position = total_samples_written;;
			lb_update_playback(libretti->playback, libretti->composition, sample_rate);
			interleave_waveform_to_stream(output_stream, libretti->playback, channel_count);

			for (int i = 0; i < stream_length; i++) 
			{
				uint32_t index = audio_clip_stream_position + i;
				if (index < total_samples) 
				{
					audio_clip.data[index] = output_stream[i];
				}
			}

			total_samples_written += stream_length;
			libretti->playback->current_play_time = (float)total_samples_written / (sample_rate * channel_count);
		}

		free(output_stream);
	}

	return audio_clip;
}

bool lb_save_audio_clip_to_wav_file(lb_AudioClip* audio_clip, const char* filename)
{
    if (!filename || !audio_clip || !audio_clip->data) {
        fprintf(stderr, "[Error] Invalid arguments passed to save_clip_to_wav.\n");
        return false;
    }

    if (audio_clip->channel_count == 0 || audio_clip->sample_rate == 0) {
        fprintf(stderr, "[Error] Clip has invalid metadata (Channels: %d, Rate: %u).\n", 
                audio_clip->channel_count, audio_clip->sample_rate);
        return false;
    }

    drwav_data_format format;
    format.container = drwav_container_riff;     
    format.format = DR_WAVE_FORMAT_PCM;          
    format.channels = audio_clip->channel_count;
    format.sampleRate = audio_clip->sample_rate;
    format.bitsPerSample = sizeof(int16_t) * 8;                   

    
    drwav wav;
    if (!drwav_init_file_write(&wav, filename, &format, NULL)) {
        fprintf(stderr, "[Error] Failed to open file '%s' for writing. Check permissions or path.\n", filename);
        return false;
    }

    drwav_uint64 framesWritten = drwav_write_pcm_frames(&wav, audio_clip->samples_per_channel, audio_clip->data);

    drwav_uninit(&wav);

    if (framesWritten != audio_clip->samples_per_channel) {
        fprintf(stderr, "[Error] Disk I/O error: Expected to write %u frames, but only wrote %llu.\n", 
                audio_clip->samples_per_channel, framesWritten);
        return false;
    }

    printf("[Success] Audio clip saved to: %s\n", filename);
    return true;
}

void lb_free_playback(lb_Playback* playback)
{
	free(playback);
	playback = 0;
}

void lb_free_composition(lb_Composition* composition)
{
	if (composition->tracks->note_events != 0)
		free(composition->tracks->note_events);
	if (composition->tempo_events != 0)
		free(composition->tempo_events);
	if (composition->lyrics_events != 0)
		free(composition->lyrics_events);
	free(composition);
	composition = 0;
}

void lb_free_libretti(lb_Libretti* libretti)
{
	if (libretti != 0)
	{
		lb_free_playback(libretti->playback);
		lb_free_composition(libretti->composition);
		libretti->playback = 0;
		libretti->composition = 0;
		free(libretti);
	}
	libretti = 0;
}