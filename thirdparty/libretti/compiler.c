#include "compiler.h"
#include "file.h"
#include "script_parse_states.h"
#include "script_validator.h"
#include "validation.h"
#include "strings.h"
#include "timing.h"
#include "notes.h"
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

void compile_composition_from_script(lb_Composition* composition, const char* script)
{
	composition->validation_statuses = validate_script(script);

	if (composition->validation_statuses == LB_VALIDATION_ALL_OK)
	{
		allocate_memory(composition, script);
		build_composition_data(composition, script);
	}
	else
	{
		printf("\n_compilation Failed.");
	}
}

void allocate_memory(lb_Composition* composition, const char* script)
{
	int read_position = 0;
	int current_track = -1;
	int note_count = 0;
	int tempo_event_count = 0;
	int lyrics_event_count = 0;
	char symbol = 0;
	bool is_reading_header = false;
	bool is_reading_value = false;
	lb_String header = lb_new_string("");

	do
	{
		symbol = script[read_position];

		switch (symbol)
		{
		case '{':
			current_track++;
			note_count = 0;
			break;
		case '[':
			is_reading_header = true;
			break;
		case ':':
			if (strcmp(header.data, "tempo") == 0)
				tempo_event_count++;
			else if (strcmp(header.data, "lyric") == 0)
				lyrics_event_count++;
			is_reading_header = false;
			is_reading_value = true;
			lb_clear_string(&header);
			break;
		case '}':
			if (note_count > 0)
				composition->tracks[current_track].note_events = malloc(sizeof(lb_NoteEvent) * note_count);
			else
				composition->tracks[current_track].note_events = calloc(1, sizeof(lb_NoteEvent));
			composition->tracks[current_track].note_count = note_count;
			break;
		case ']':
			is_reading_value = false;
			break;
		default:
			if (!is_reading_value)
			{
				if (is_reading_header)
					lb_append_string(&header, symbol);
				else if (symbol >= 'A' && symbol <= 'G' ||
					symbol == 'R')
					note_count++;
			}
		}
		read_position++;
	} while (symbol != 0);

	composition->tempo_events = malloc(sizeof(lb_TempoEvent) * tempo_event_count);
	composition->tempo_event_count = tempo_event_count;
	composition->lyrics_events = malloc(sizeof(lb_LyricsEvent) * lyrics_event_count);
	composition->lyrics_event_count = lyrics_event_count;
	composition->track_count = current_track + 1;
	lb_free_string(&header);
}

void build_composition_data(lb_Composition* composition, const char* script)
{
	uint32_t read_position = 0;
	uint32_t current_note = 0;
	uint8_t current_track = 0;
	uint32_t current_tempo_event = 0;
	uint32_t current_lyrics_event = 0;

	double currentTime_s = 0.0;
	uint16_t bar_count = 0;

	uint8_t time_sig_lower = 0;
	uint8_t time_sig_upper = 0;
	uint8_t octave = 0;
	uint16_t tempo = 0;
	uint16_t dynamic = 0;
	int8_t panning = 0;
	uint8_t timbre = 0;
	uint16_t cue = 0;
	double duration = 0.0;
	bool tuplet_is_opened = false;
	bool slur_is_opened = false;
	bool is_reading_crescendo = false;
	bool is_reading_diminuendo = false;
	bool has_fractional_duration = false;
	bool has_dotted_duration = false;

	bool is_in_crescendo_state = false;
	bool is_in_diminuendo_state = false;
	uint32_t gradual_dynamics_note_indices[256] = {0};
	uint16_t gradual_dynamics_note_count = 0;

	lb_Note note;
	lb_Effects effects;
	effects.echo.decay = 0.0;
	effects.echo.delay = 0.0;
	effects.reverb.damping = 0.0;
	effects.reverb.pre_delay = 0.0;
	effects.reverb.room_size = 0.0;
	effects.vibrato.extent = 0.0;
	effects.vibrato.rate = 0.0;
	effects.crossfading = 0.0;
	effects.pitch_blend = 0.0;

	for (int i = 0; i < 10; i++)
		effects.eq[i].level = 0;

	char note_to_play;

	uint8_t parse_state = LB_PARSE_STATE_READING_NOTHING;
	uint8_t previous_parse_state = 0;
	lb_String header = lb_new_string("");
	lb_String value = lb_new_string("");

	lb_String duration_string = lb_new_string("");

	do
	{
		switch (script[read_position])
		{
		case '{':
			parse_state = LB_PARSE_STATE_READING_TRACK_SCOPE;
			break;
		case '[':
			previous_parse_state = parse_state;
			parse_state = LB_PARSE_STATE_READING_HEADER;
			break;
		case '_':
			tuplet_is_opened = !tuplet_is_opened;
			break;
		case '~':
			slur_is_opened = !slur_is_opened;
			break;
		case '<':
			if (is_reading_crescendo)
			{
				is_in_crescendo_state = !is_in_crescendo_state;

				if (is_in_crescendo_state)
					gradual_dynamics_note_count = 0;
			}

			is_reading_crescendo = !is_reading_crescendo;
			break;
		case '>':
			if (parse_state == LB_PARSE_STATE_READING_NOTE_DURATION)
			{
				parse_state = LB_PARSE_STATE_READING_DYNAMIC_ACCENT;
			}
			else
			{
				if (is_reading_diminuendo)
				{
					is_in_diminuendo_state = !is_in_diminuendo_state;

					if (is_in_diminuendo_state)
						gradual_dynamics_note_count = 0;
				}

				is_reading_diminuendo = !is_reading_diminuendo;
			}
			break;
		case '*':
			if (parse_state == LB_PARSE_STATE_READING_NOTE_DURATION)
				parse_state = LB_PARSE_STATE_READING_STACCATO;
			break;
		case '|':
			bar_count++;
			duration = (double)time_sig_upper;
			duration *= (60.0 / (double)tempo) / (4.0 / (double)time_sig_lower);
			currentTime_s = duration * bar_count;
			break;
		case ' ':
			if (is_reading_crescendo)
				is_reading_crescendo = false;

			if (is_reading_diminuendo)
				is_reading_diminuendo = false;

			if (parse_state == LB_PARSE_STATE_READING_NOTE_DURATION ||
				parse_state == LB_PARSE_STATE_READING_DYNAMIC_ACCENT ||
				parse_state == LB_PARSE_STATE_READING_STACCATO)
			{
				if (parse_state == LB_PARSE_STATE_READING_DYNAMIC_ACCENT)
					note.dynamic = dynamic * 2;
				else
					note.dynamic = dynamic;

				if (parse_state == LB_PARSE_STATE_READING_STACCATO)
					note.articulation = LB_ARTICULATION_STACCATO;
				else if (slur_is_opened)
					note.articulation = LB_ARTICULATION_SLUR;
				else
					note.articulation = LB_ARTICULATION_NORMAL;

				note.cue = cue;
				note.panning = panning;
				note.timbre = timbre;
				note.effects = effects;

				composition->tracks[current_track].note_events[current_note].note = note;
				composition->tracks[current_track].note_events[current_note].start_time = currentTime_s;

				if (is_in_crescendo_state || is_in_diminuendo_state)
				{
					gradual_dynamics_note_indices[gradual_dynamics_note_count] = current_note;
					gradual_dynamics_note_count++;
				}

				if (has_fractional_duration)
					duration = 1.0 / atoi(duration_string.data);
				else if (has_dotted_duration)
					;
				else
					duration = atoi(duration_string.data);

				lb_clear_string(&duration_string);

				if (tuplet_is_opened)
					duration = (duration * 2.0) / 3.0;

				double seconds_per_beat = 60.0 / (double)tempo;
				double beat_length = 4.0 / (double)time_sig_lower;
				seconds_per_beat /= beat_length;
				duration *= seconds_per_beat;
				currentTime_s += duration;
				parse_state = LB_PARSE_STATE_READING_TRACK_SCOPE;
				current_note++;
			}

			if (parse_state == LB_PARSE_STATE_IGNORING_FIRST_SPACE_IN_VALUE)
			{
				parse_state = LB_PARSE_STATE_READING_VALUE;
			}
			else if (parse_state == LB_PARSE_STATE_READING_HEADER)
			{
				lb_append_string(&header, script[read_position]);
			}
			else if (parse_state == LB_PARSE_STATE_READING_VALUE)
			{
				lb_append_string(&value, script[read_position]);
			}
			break;
		case ':':
			parse_state = LB_PARSE_STATE_IGNORING_FIRST_SPACE_IN_VALUE;
			break;
		case ']':
			if (strcmp(header.data, "name") == 0)
			{
				int value_read_position = 0;
				while (value.data[value_read_position] != 0)
				{
					composition->name[value_read_position] = value.data[value_read_position];
					value_read_position++;
				}
			}
			else if (strcmp(header.data, "artist") == 0)
			{
				int value_read_position = 0;
				while (value.data[value_read_position] != 0)
				{
					composition->artist[value_read_position] = value.data[value_read_position];
					value_read_position++;
				}
			}
			else if (strcmp(header.data, "key signature") == 0)
			{
				if (strcmp(value.data, "C major") == 0)
					composition->key_signature = LB_KEY_SIGNATURE_C_MAJOR;
				else if (strcmp(value.data, "G major") == 0)
					composition->key_signature = LB_KEY_SIGNATURE_G_MAJOR;
				else if (strcmp(value.data, "D major") == 0)
					composition->key_signature = LB_KEY_SIGNATURE_D_MAJOR;
				else if (strcmp(value.data, "A major") == 0)
					composition->key_signature = LB_KEY_SIGNATURE_A_MAJOR;
				else if (strcmp(value.data, "E major") == 0)
					composition->key_signature = LB_KEY_SIGNATURE_E_MAJOR;
				else if (strcmp(value.data, "B major") == 0)
					composition->key_signature = LB_KEY_SIGNATURE_B_MAJOR;
				else if (strcmp(value.data, "F# major") == 0)
					composition->key_signature = LB_KEY_SIGNATURE_Fs_MAJOR;
				else if (strcmp(value.data, "Gb major") == 0)
					composition->key_signature = LB_KEY_SIGNATURE_Gb_MAJOR;
				else if (strcmp(value.data, "Db major") == 0)
					composition->key_signature = LB_KEY_SIGNATURE_Db_MAJOR;
				else if (strcmp(value.data, "Ab major") == 0)
					composition->key_signature = LB_KEY_SIGNATURE_Ab_MAJOR;
				else if (strcmp(value.data, "Eb major") == 0)
					composition->key_signature = LB_KEY_SIGNATURE_Eb_MAJOR;
				else if (strcmp(value.data, "Bb major") == 0)
					composition->key_signature = LB_KEY_SIGNATURE_Bb_MAJOR;
				else if (strcmp(value.data, "F major") == 0)
					composition->key_signature = LB_KEY_SIGNATURE_F_MAJOR;
				else if (strcmp(value.data, "Cb major") == 0)
					composition->key_signature = LB_KEY_SIGNATURE_Cb_MAJOR;
				else if (strcmp(value.data, "C# major") == 0)
					composition->key_signature = LB_KEY_SIGNATURE_Cs_MAJOR;
				else if (strcmp(value.data, "A minor") == 0)
					composition->key_signature = LB_KEY_SIGNATURE_A_MINOR;
				else if (strcmp(value.data, "E minor") == 0)
					composition->key_signature = LB_KEY_SIGNATURE_E_MINOR;
				else if (strcmp(value.data, "B minor") == 0)
					composition->key_signature = LB_KEY_SIGNATURE_B_MINOR;
				else if (strcmp(value.data, "F# minor") == 0)
					composition->key_signature = LB_KEY_SIGNATURE_Fs_MINOR;
				else if (strcmp(value.data, "C# minor") == 0)
					composition->key_signature = LB_KEY_SIGNATURE_Cs_MINOR;
				else if (strcmp(value.data, "G# minor") == 0)
					composition->key_signature = LB_KEY_SIGNATURE_Gs_MINOR;
				else if (strcmp(value.data, "D# minor") == 0)
					composition->key_signature = LB_KEY_SIGNATURE_Ds_MINOR;
				else if (strcmp(value.data, "Eb minor") == 0)
					composition->key_signature = LB_KEY_SIGNATURE_Eb_MINOR;
				else if (strcmp(value.data, "Bb minor") == 0)
					composition->key_signature = LB_KEY_SIGNATURE_Bb_MINOR;
				else if (strcmp(value.data, "F minor") == 0)
					composition->key_signature = LB_KEY_SIGNATURE_F_MINOR;
				else if (strcmp(value.data, "C minor") == 0)
					composition->key_signature = LB_KEY_SIGNATURE_C_MINOR;
				else if (strcmp(value.data, "G minor") == 0)
					composition->key_signature = LB_KEY_SIGNATURE_G_MINOR;
				else if (strcmp(value.data, "D minor") == 0)
					composition->key_signature = LB_KEY_SIGNATURE_D_MINOR;
			}
			else if (strcmp(header.data, "time signature") == 0)
			{
				int value_read_position = 0;
				lb_String upper = lb_new_string("");
				lb_String lower = lb_new_string("");

				while (value.data[value_read_position] != 0 &&
					value.data[value_read_position] != '/')
				{
					lb_append_string(&upper, value.data[value_read_position]);
					value_read_position++;
				}

				value_read_position++;

				while (value.data[value_read_position] != 0)
				{
					lb_append_string(&lower, value.data[value_read_position]);
					value_read_position++;
				}

				time_sig_upper = atoi(upper.data);
				time_sig_lower = atoi(lower.data);

				composition->time_signature[0] = time_sig_upper;
				composition->time_signature[1] = time_sig_lower;

				lb_free_string(&upper);
				lb_free_string(&lower);
			}
			else if (strcmp(header.data, "tempo") == 0)
			{
				tempo = atoi(value.data);
				if (tempo == 0)
				{
					if (strcmp(value.data, "largo") == 0)
						tempo = LB_BPM_LARGO;
					else if (strcmp(value.data, "adagio") == 0)
						tempo = LB_BPM_ADAGIO;
					else if (strcmp(value.data, "adante") == 0)
						tempo = LB_BPM_ADANTE;
					else if (strcmp(value.data, "moderato") == 0)
						tempo = LB_BPM_MODERATO;
					else if (strcmp(value.data, "allegro") == 0)
						tempo = LB_BPM_ALLEGRO;
					else if (strcmp(value.data, "presto") == 0)
						tempo = LB_BPM_PRESTO;
				}
				composition->tempo_events[current_tempo_event].tempo = tempo;
				composition->tempo_events[current_tempo_event].start_time = currentTime_s;
				current_tempo_event++;
			}
			else if (strcmp(header.data, "dynamic") == 0)
			{
				uint16_t previous_dynamic = dynamic;

				if (strcmp(value.data, "ppp") == 0)
					dynamic = LB_DYNAMIC_PPP;
				else if (strcmp(value.data, "pp") == 0)
					dynamic = LB_DYNAMIC_PP;
				else if (strcmp(value.data, "p") == 0)
					dynamic = LB_DYNAMIC_P;
				else if (strcmp(value.data, "mp") == 0)
					dynamic = LB_DYNAMIC_MP;
				else if (strcmp(value.data, "mf") == 0)
					dynamic = LB_DYNAMIC_MF;
				else if (strcmp(value.data, "f") == 0)
					dynamic = LB_DYNAMIC_F;
				else if (strcmp(value.data, "ff") == 0)
					dynamic = LB_DYNAMIC_FF;
				else if (strcmp(value.data, "fff") == 0)
					dynamic = LB_DYNAMIC_FFF;


				if (!is_in_crescendo_state && !is_in_diminuendo_state)
				{
					for (int i = 0; i < gradual_dynamics_note_count; i++)
					{
						uint32_t note_index = gradual_dynamics_note_indices[i];
						float gradual_dynamic_delta = (float)(dynamic - previous_dynamic) / (float)gradual_dynamics_note_count;
						uint16_t gradual_dynamic = (uint16_t)((float)previous_dynamic + (gradual_dynamic_delta * i));
						composition->tracks[current_track].note_events[note_index].note.dynamic = gradual_dynamic;
					}

					gradual_dynamics_note_count = 0;
				}

			}
			else if (strcmp(header.data, "reverb") == 0)
			{
				float reverb_value = atof(value.data);
				if (reverb_value == 0)
				{
					if (strcmp(value.data, "standard") == 0)
					{
						effects.reverb.damping = 50;
						effects.reverb.pre_delay = 10;
						effects.reverb.room_size = 50;
					}
					else if (strcmp(value.data, "none") == 0)
					{
						effects.reverb.damping = 0;
						effects.reverb.pre_delay = 0;
						effects.reverb.room_size = 0;
					}
				}
			}
			else if (strcmp(header.data, "vibrato") == 0)
			{
				if (strcmp(value.data, "standard") == 0)
				{
					effects.vibrato.extent = 100;
					effects.vibrato.rate = 8;
				}
				else if (strcmp(value.data, "none") == 0)
				{
					effects.vibrato.extent = 0;
					effects.vibrato.rate = 0;
				}
			}
			else if (strcmp(header.data, "panning") == 0)
			{
				float panning_value = atof(value.data);
				if (panning_value == 0)
				{
					if (strcmp(value.data, "far left") == 0)
						panning = LB_PANNING_REAR_LEFT;
					else if (strcmp(value.data, "left") == 0)
						panning = LB_PANNING_FRONT_LEFT;
					else if (strcmp(value.data, "mono") == 0)
						panning = LB_PANNING_CENTRE;
					else if (strcmp(value.data, "right") == 0)
						panning = LB_PANNING_FRONT_RIGHT;
					else if (strcmp(value.data, "far right") == 0)
						panning = LB_PANNING_REAR_RIGHT;
				}
				else
				{
					panning = panning_value * 127;
				}
			}
			else if (strcmp(header.data, "timbre") == 0)
			{
				if (strcmp(value.data, "square wave") == 0)
					timbre = LB_TIMBRE_SQUARE_WAVE;
				else if (strcmp(value.data, "sine wave") == 0)
					timbre = LB_TIMBRE_SINE_WAVE;
				else if (strcmp(value.data, "triangle wave") == 0)
					timbre = LB_TIMBRE_TRIANGLE_WAVE;
				else if (strcmp(value.data, "sawtooth wave") == 0)
					timbre = LB_TIMBRE_SAWTOOTH_WAVE;
				else if (strcmp(value.data, "pulse 10") == 0)
					timbre = LB_TIMBRE_PULSE_10;
				else if (strcmp(value.data, "pulse 25") == 0)
					timbre = LB_TIMBRE_PULSE_25;
				else if (strcmp(value.data, "noise") == 0)
					timbre = LB_TIMBRE_NOISE;
				else
				{
#ifdef _DEBUG
					lb_String filename = lb_new_string("../../../../../demos/data/samples/");
#else
					lb_String filename = lb_new_string("data/samples/");
#endif
					lb_String extension = lb_new_string(".pcm");
					timbre = LB_TIMBRE_SAMPLE;
					strcat(filename.data, value.data);
					strcat(filename.data, extension.data);
					lb_BinaryS16 binary = load_binary_s16_from_file(filename.data);
					float sample_progress_delta = (float)binary.size / (float)DEFAULT_STREAM_SAMPLE_SIZE;

					int16_t debug_binary_data[2048] = { 0 };

					for (int i = 0; i < binary.size; i++)
						debug_binary_data[i] = binary.data[i];

					for (int i = 0; i < DEFAULT_STREAM_SAMPLE_SIZE; i++)
						note.sample[i] = binary.data[(int)(sample_progress_delta * i)];

					lb_free_string(&filename);
					lb_free_string(&extension);
				}
			}
			else if (strcmp(header.data, "octave") == 0)
			{
				octave = atoi(value.data);
			}
			else if (strcmp(header.data, "loop") == 0)
			{
				composition->loop_timestamp = currentTime_s;
				composition->loop_count = atoi(value.data);
				if (strcmp(value.data, "infinity") == 0 || 
					composition->loop_count > pow(2, (sizeof composition->loop_count) * 8) - 1)
					composition->loop_count = pow(2, (sizeof composition->loop_count) * 8) - 1;
			}
			else if (strcmp(header.data, "cue") == 0)
			{
				cue = atoi(value.data);
			}
			else if (strcmp(header.data, "echo") == 0)
			{
				if (strcmp(value.data, "standard") == 0)
				{
					effects.echo.decay = 50;
					effects.echo.delay = 1000;
				}
				else if (strcmp(value.data, "none") == 0)
				{
					effects.echo.decay = 0;
					effects.echo.delay = 0;
				}
			}
			else if (strcmp(header.data, "lyric") == 0)
			{
				//audio->lyrics_events[current_lyrics_event].lyrics = malloc(sizeof(char) * value.capacity);
				strcpy(composition->lyrics_events[current_lyrics_event].lyrics, value.data);
				composition->lyrics_events[current_lyrics_event].start_time = currentTime_s;
				current_lyrics_event++;
			}
			else if (strcmp(header.data, "eq") == 0)
			{
				int value_read_position = 0;
				lb_String frequency_string = lb_new_string("");
				lb_String level_string = lb_new_string("");

				while (value.data[value_read_position] != 0 &&
					value.data[value_read_position] != ';')
				{
					lb_append_string(&frequency_string, value.data[value_read_position]);
					value_read_position++;
				}

				value_read_position++;

				while (value.data[value_read_position] != 0)
				{
					lb_append_string(&level_string, value.data[value_read_position]);
					value_read_position++;
				}

				if (strcmp(value.data, "none") == 0)
				{
					for (int i = 0; i < 10; i++)
						effects.eq[i].level = 0;
				}
				else
				{
					if (strcmp(frequency_string.data, "31Hz") == 0)
						effects.eq[0].level = atof(level_string.data);
					if (strcmp(frequency_string.data, "62Hz") == 0)
						effects.eq[1].level = atof(level_string.data);
					if (strcmp(frequency_string.data, "125Hz") == 0)
						effects.eq[2].level = atof(level_string.data);
					if (strcmp(frequency_string.data, "250Hz") == 0)
						effects.eq[3].level = atof(level_string.data);
					if (strcmp(frequency_string.data, "500Hz") == 0)
						effects.eq[4].level = atof(level_string.data);
					if (strcmp(frequency_string.data, "1kHz") == 0)
						effects.eq[5].level = atof(level_string.data);
					if (strcmp(frequency_string.data, "2kHz") == 0)
						effects.eq[6].level = atof(level_string.data);
					if (strcmp(frequency_string.data, "4kHz") == 0)
						effects.eq[7].level = atof(level_string.data);
					if (strcmp(frequency_string.data, "8kHz") == 0)
						effects.eq[8].level = atof(level_string.data);
					if (strcmp(frequency_string.data, "16kHz") == 0)
						effects.eq[9].level = atof(level_string.data);
				}

				lb_free_string(&frequency_string);
				lb_free_string(&level_string);
			}
			else if (strcmp(header.data, "crossfading") == 0)
			{
				effects.crossfading = atoi(value.data);
				if (strcmp(value.data, "none") == 0)
					effects.crossfading = 0;
			}
			else if (strcmp(header.data, "pitch blend") == 0)
			{
				effects.pitch_blend = atof(value.data);
				if (strcmp(value.data, "none") == 0)
					effects.pitch_blend = 0;
			}
			lb_clear_string(&header);
			lb_clear_string(&value);
			parse_state = previous_parse_state;
			break;
		case '}':
			composition->time_length = currentTime_s;
			currentTime_s = 0.0;
			current_note = 0;
			bar_count = 0;
			current_track++;
			parse_state = LB_PARSE_STATE_READING_NOTHING;
			break;
		default:
			if (parse_state == LB_PARSE_STATE_READING_HEADER)
			{
				lb_append_string(&header, script[read_position]);
			}
			else if (parse_state == LB_PARSE_STATE_READING_VALUE)
			{
				lb_append_string(&value, script[read_position]);
			}
			else if (parse_state == LB_PARSE_STATE_READING_TRACK_SCOPE)
			{
				if ((script[read_position] >= 'A' && script[read_position] <= 'G') ||
					script[read_position] == 'R')
				{
					parse_state = LB_PARSE_STATE_READING_NOTE_FREQUENCY;
					note_to_play = script[read_position];
					tune_by_key_signature(composition->key_signature, &note_to_play);
					assign_frequency_from_note_char(&note.key, octave, note_to_play);
				}
				else if (script[read_position] == '-')
				{
					if (parse_state != LB_PARSE_STATE_READING_VALUE)
						octave--;
				}
				else if (script[read_position] == '+')
				{
					if (parse_state != LB_PARSE_STATE_READING_VALUE)
						octave++;
				}
			}
			else if ((parse_state == LB_PARSE_STATE_READING_NOTE_FREQUENCY || parse_state == LB_PARSE_STATE_READING_NOTE_ACCIDENTAL) &&
				(script[read_position] >= '1' && script[read_position] <= '9'))
			{
				has_fractional_duration = false;
				has_dotted_duration = false;
				parse_state = LB_PARSE_STATE_READING_NOTE_DURATION;
				lb_append_string(&duration_string, script[read_position]);
			}
			else if ((parse_state == LB_PARSE_STATE_READING_NOTE_FREQUENCY || parse_state == LB_PARSE_STATE_READING_NOTE_ACCIDENTAL) &&
				(script[read_position] == '/'))
			{
				has_fractional_duration = true;
				parse_state = LB_PARSE_STATE_READING_NOTE_DURATION;
			}
			else if (parse_state == LB_PARSE_STATE_READING_NOTE_DURATION && script[read_position] == '.')
			{
				has_dotted_duration = true;

				if (has_fractional_duration)
					duration = 1.0 / atoi(duration_string.data);
				else
					duration = atoi(duration_string.data);

				duration += duration / 2.0;
			}
			else if (parse_state == LB_PARSE_STATE_READING_NOTE_DURATION &&
				(script[read_position] >= '0' && script[read_position] <= '9'))
			{
				lb_append_string(&duration_string, script[read_position]);
			}
			else if (parse_state == LB_PARSE_STATE_READING_NOTE_FREQUENCY &&
				(script[read_position] == '#' || script[read_position] == 'b' ||
					script[read_position] == 'n'))
			{
				parse_state = LB_PARSE_STATE_READING_NOTE_ACCIDENTAL;
				tune_by_accidental(&note.key, octave, script[read_position], note_to_play);
			}
		}
		read_position++;
	} while (script[read_position] != 0);

	lb_free_string(&header);
	lb_free_string(&value);
	lb_free_string(&duration_string);
}

void tune_by_key_signature(uint8_t key_signature, char* note_char)
{

	if (key_signature == LB_KEY_SIGNATURE_C_MAJOR || key_signature == LB_KEY_SIGNATURE_A_MINOR)
	{
		;
	}
	else if (key_signature == LB_KEY_SIGNATURE_G_MAJOR || key_signature == LB_KEY_SIGNATURE_E_MINOR)
	{
		switch (*note_char)
		{
			case 'F': *note_char = 'f'; break;
		}
	}
	else if (key_signature == LB_KEY_SIGNATURE_D_MAJOR || key_signature == LB_KEY_SIGNATURE_B_MINOR)
	{
		switch (*note_char)
		{
			case 'F': *note_char = 'f'; break;
			case 'C': *note_char = 'c'; break;
		}
	}
	else if (key_signature == LB_KEY_SIGNATURE_A_MAJOR || key_signature == LB_KEY_SIGNATURE_Fs_MINOR)
	{
		switch (*note_char)
		{
			case 'F': *note_char = 'f'; break;
			case 'C': *note_char = 'c'; break;
			case 'G': *note_char = 'g'; break;
		}
	}
	else if (key_signature == LB_KEY_SIGNATURE_E_MAJOR || key_signature == LB_KEY_SIGNATURE_Cs_MINOR)
	{
		switch (*note_char)
		{
			case 'F': *note_char = 'f'; break;
			case 'C': *note_char = 'c'; break;
			case 'G': *note_char = 'g'; break;
			case 'D': *note_char = 'e'; break;
		}
	}
	else if (key_signature == LB_KEY_SIGNATURE_B_MAJOR || key_signature == LB_KEY_SIGNATURE_Cb_MAJOR ||
		key_signature == LB_KEY_SIGNATURE_Gs_MINOR)
	{
		switch (*note_char)
		{
			case 'F': *note_char = 'f'; break;
			case 'C': *note_char = 'c'; break;
			case 'G': *note_char = 'g'; break;
			case 'D': *note_char = 'e'; break;
			case 'A': *note_char = 'b'; break;
		}
	}
	else if (key_signature == LB_KEY_SIGNATURE_Fs_MAJOR || key_signature == LB_KEY_SIGNATURE_Ds_MINOR)
	{
		switch (*note_char)
		{
			case 'F': *note_char = 'f'; break;
			case 'C': *note_char = 'c'; break;
			case 'G': *note_char = 'g'; break;
			case 'D': *note_char = 'e'; break;
			case 'A': *note_char = 'b'; break;
			case 'E': *note_char = 'F'; break;
		}
	}
	else if (key_signature == LB_KEY_SIGNATURE_Gb_MAJOR || key_signature == LB_KEY_SIGNATURE_Eb_MINOR)
	{
		switch (*note_char)
		{
			case 'B': *note_char = 'b'; break;
			case 'E': *note_char = 'e'; break;
			case 'A': *note_char = 'g'; break;
			case 'D': *note_char = 'c'; break;
			case 'G': *note_char = 'f'; break;
			case 'C': *note_char = 'B'; break;
		}
	}
	else if (key_signature == LB_KEY_SIGNATURE_Db_MAJOR || key_signature == LB_KEY_SIGNATURE_Cs_MAJOR ||
		key_signature == LB_KEY_SIGNATURE_Bb_MINOR)
	{
		switch (*note_char)
		{
			case 'B': *note_char = 'b'; break;
			case 'E': *note_char = 'e'; break;
			case 'A': *note_char = 'g'; break;
			case 'D': *note_char = 'c'; break;
			case 'G': *note_char = 'f'; break;
		}
	}
	else if (key_signature == LB_KEY_SIGNATURE_Ab_MAJOR || key_signature == LB_KEY_SIGNATURE_F_MINOR)
	{
		switch (*note_char)
		{
			case 'B': *note_char = 'b'; break;
			case 'E': *note_char = 'e'; break;
			case 'A': *note_char = 'g'; break;
			case 'D': *note_char = 'c'; break;
		}
	}
	else if (key_signature == LB_KEY_SIGNATURE_Eb_MAJOR || key_signature == LB_KEY_SIGNATURE_C_MINOR)
	{
		switch (*note_char)
		{
			case 'B': *note_char = 'b'; break;
			case 'E': *note_char = 'e'; break;
			case 'A': *note_char = 'g'; break;
		}
	}
	else if (key_signature == LB_KEY_SIGNATURE_Bb_MAJOR || key_signature == LB_KEY_SIGNATURE_G_MINOR)
	{
		switch (*note_char)
		{
			case 'B': *note_char = 'b'; break;
			case 'E': *note_char = 'e'; break;
		}
	}
	else if (key_signature == LB_KEY_SIGNATURE_F_MAJOR || key_signature == LB_KEY_SIGNATURE_D_MINOR)
	{
		switch (*note_char)
		{
			case 'B': *note_char = 'b'; break;
		}
	}
}

void assign_frequency_from_note_char(float* frequency, uint8_t octave, char note_char)
{
	switch (note_char)
	{
		case 'A': *frequency = LB_KEY_A0 * pow(2, octave); break;
		case 'B': *frequency = LB_KEY_B0 * pow(2, octave); break;
		case 'C': *frequency = LB_KEY_C1 * pow(2, octave - 1); break;
		case 'D': *frequency = LB_KEY_D1 * pow(2, octave - 1); break;
		case 'E': *frequency = LB_KEY_E1 * pow(2, octave - 1); break;
		case 'F': *frequency = LB_KEY_F1 * pow(2, octave - 1); break;
		case 'G': *frequency = LB_KEY_G1 * pow(2, octave - 1); break;
			/*Flats and sharps char codes reserved for compiler.*/
		case 'b': *frequency = LB_KEY_Bf0 * pow(2, octave); break;
		case 'c': *frequency = LB_KEY_Cs1 * pow(2, octave - 1); break;
		case 'e': *frequency = LB_KEY_Ef1 * pow(2, octave - 1); break;
		case 'f': *frequency = LB_KEY_Fs1 * pow(2, octave - 1); break;
		case 'g': *frequency = LB_KEY_Gs1 * pow(2, octave - 1); break;
			/*Rest value.*/
		case 'R': *frequency = LB_KEY_R; break;
	}
}

void tune_by_accidental(float* frequency, uint8_t octave, char script_char, char note_char)
{
	switch (script_char)
	{
	case '#':
		switch (note_char)
		{
			case 'A': *frequency = LB_KEY_Bf0 * pow(2, octave); break;
			case 'B': *frequency = LB_KEY_C1 * pow(2, octave - 1); break;
			case 'C': *frequency = LB_KEY_Cs1 * pow(2, octave - 1); break;
			case 'D': *frequency = LB_KEY_Ef1 * pow(2, octave - 1); break;
			case 'E': *frequency = LB_KEY_F1 * pow(2, octave - 1); break;
			case 'F': *frequency = LB_KEY_Fs1 * pow(2, octave - 1); break;
			case 'G': *frequency = LB_KEY_Gs1 * pow(2, octave - 1); break;
		}
		break;
	case 'b':
		switch (note_char)
		{
			case 'A': *frequency = LB_KEY_Gs1 * pow(2, octave - 1); break;
			case 'B': *frequency = LB_KEY_Bf0 * pow(2, octave); break;
			case 'C': *frequency = LB_KEY_B0 * pow(2, octave); break;
			case 'D': *frequency = LB_KEY_Cs1 * pow(2, octave - 1); break;
			case 'E': *frequency = LB_KEY_Ef1 * pow(2, octave - 1); break;
			case 'F': *frequency = LB_KEY_E1 * pow(2, octave - 1); break;
			case 'G': *frequency = LB_KEY_Fs1 * pow(2, octave - 1); break;
		}
		break;
	case 'n':
		switch (note_char)
		{
			case 'b': *frequency = LB_KEY_B0 * pow(2, octave); break;
			case 'c': *frequency = LB_KEY_C1 * pow(2, octave - 1); break;
			case 'e': *frequency = LB_KEY_E1 * pow(2, octave - 1); break;
			case 'f': *frequency = LB_KEY_F1 * pow(2, octave - 1); break;
			case 'g': *frequency = LB_KEY_G1 * pow(2, octave - 1); break;
		}
		break;
	}
}