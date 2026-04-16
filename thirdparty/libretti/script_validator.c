#include "script_validator.h"
#include "validation.h"
#include "script_parse_states.h"
#include "constants.h"
#include "strings.h"
#include "file.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int validate_script(const char* script)
{
	int validation_statuses = LB_VALIDATION_ALL_OK;
	int read_position = 0;
	int line_position = 1;
	int column_position = 1;
	int track_scope_count = 0;
	int unclosed_headers = 0;
	int unclosed_track_scopes = 0;
	uint8_t time_sig_lower = 0;
	uint8_t time_sig_upper = 0;
	double beats_in_a_bar = 0;
	unsigned int previous_bar_count = 0;
	unsigned int current_bar_count = 0;
	uint8_t octave = 0;
	uint8_t tempo = 0;
	uint8_t parse_state = LB_PARSE_STATE_READING_NOTHING;
	uint8_t previous_parse_state = 0;

	bool tuplet_is_unclosed = false;
	bool slur_is_unclosed = false;

	bool is_reading_crescendo = false;
	bool is_reading_diminuendo = false;
	int unclosed_crescendos = 0;
	int unclosed_diminuendos = 0;

	lb_String header = lb_new_string("");
	lb_String value = lb_new_string("");
	lb_String debug = lb_new_string("");

	double duration = 0.0;
	bool has_fractional_duration = false;
	lb_String duration_string = lb_new_string("");

	printf("\n\t\t_libretti Script Validation Statuses");
	printf("\n\n");

	do
	{
		char symbol = script[read_position];

		lb_append_string(&debug, symbol);
		if (!validate_symbol(symbol, parse_state))
		{
			printf("Error 0x%X: \tINVALID_USE_OF_SYMBOL '%c' at position %d, line %d, column %d.\n", 
				LB_VALIDATION_INVALID_USE_OF_SYMBOL, 
				symbol,
				read_position,
				line_position,
				column_position);
			validation_statuses |= LB_VALIDATION_INVALID_USE_OF_SYMBOL;
		}
		else
		{
			switch (symbol)
			{
			case '{':
				parse_state = LB_PARSE_STATE_READING_TRACK_SCOPE;
				track_scope_count++;
				unclosed_track_scopes++;
				break;
			case '[':
				previous_parse_state = parse_state;
				parse_state = LB_PARSE_STATE_READING_HEADER;
				unclosed_headers++;
				break;
			case '_':
				tuplet_is_unclosed = !tuplet_is_unclosed;
				break;
			case '~':
				slur_is_unclosed = !slur_is_unclosed;
				break;
			case '<':
				if (!is_reading_crescendo)
				{
					is_reading_crescendo = true;
				}
				else
				{
					if (unclosed_crescendos > 0)
						unclosed_crescendos--;
					else
						unclosed_crescendos++;
					is_reading_crescendo = false;
				}
				break;
			case '>':
				if (!is_reading_diminuendo)
				{
					is_reading_diminuendo = true;
				}
				else
				{
					if (unclosed_diminuendos > 0)
						unclosed_diminuendos--;
					else
						unclosed_diminuendos++;
					is_reading_diminuendo = false;
				}
				break;
			case ' ':
				if (is_reading_crescendo)
					is_reading_crescendo = false;

				if (is_reading_diminuendo)
					is_reading_diminuendo = false;

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
				else if (parse_state == LB_PARSE_STATE_READING_NOTE_DURATION)
				{
					if (has_fractional_duration)
						duration = 1.0 / atoi(duration_string.data);
					else
						duration = atoi(duration_string.data);

					if (tuplet_is_unclosed)
						duration = (duration * 2.0) / 3.0;

					lb_clear_string(&duration_string);
					beats_in_a_bar += duration;
					parse_state = LB_PARSE_STATE_READING_TRACK_SCOPE;
				}
				break;
			case '|':
				if (beats_in_a_bar != 0)
					if (round(beats_in_a_bar) != time_sig_upper)
					{
						printf("Error 0x%X: \tBEATS_DO_NOT_MATCH_TIME_SIG %d != %d at position %d, line %d, column %d.\n",
							LB_VALIDATION_BEATS_DO_NOT_MATCH_TIME_SIG,
							beats_in_a_bar,
							time_sig_upper,
							read_position,
							line_position,
							column_position);
						validation_statuses |= LB_VALIDATION_BEATS_DO_NOT_MATCH_TIME_SIG;
					}
				beats_in_a_bar = 0;
				current_bar_count++;
				break;
			case ':':
				parse_state = LB_PARSE_STATE_IGNORING_FIRST_SPACE_IN_VALUE;
				if (strcmp(header.data, "comment") != 0 &&
					strcmp(header.data, "name") != 0 &&
					strcmp(header.data, "artist") != 0 &&
					strcmp(header.data, "lyric") != 0 &&
					strcmp(header.data, "time signature") != 0 &&
					strcmp(header.data, "key signature") != 0 &&
					strcmp(header.data, "tempo") != 0 &&
					strcmp(header.data, "timbre") != 0 &&
					strcmp(header.data, "octave") != 0 &&
					strcmp(header.data, "dynamic") != 0 &&
					strcmp(header.data, "loop") != 0 &&
					strcmp(header.data, "segment") != 0 &&
					strcmp(header.data, "end") != 0 &&
					strcmp(header.data, "cue") != 0 &&
					strcmp(header.data, "panning") != 0 &&
					strcmp(header.data, "reverb") != 0 &&
					strcmp(header.data, "echo") != 0 &&
					strcmp(header.data, "eq") != 0 &&
					strcmp(header.data, "vibrato") != 0 &&
					strcmp(header.data, "flanging") != 0 &&
					strcmp(header.data, "crossfading") != 0 &&
					strcmp(header.data, "pitch blend") != 0)
				{
					printf("Error 0x%X: \tUNEXPECTED_HEADER_NAME '%s' at position %d, line %d, column %d.\n",
						LB_VALIDATION_UNEXPECTED_HEADER_NAME,
						header.data,
						read_position,
						line_position,
						column_position);
					validation_statuses |= LB_VALIDATION_UNEXPECTED_HEADER_NAME;
				}
				break;
			case ']':
				if (strcmp(header.data, "time signature") == 0)
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
				}
				else if (strcmp(header.data, "key signature") == 0)
				{

					if (strcmp(value.data, "C major") != 0 &&
						strcmp(value.data, "G major") != 0 &&
						strcmp(value.data, "D major") != 0 &&
						strcmp(value.data, "A major") != 0 &&
						strcmp(value.data, "E major") != 0 &&
						strcmp(value.data, "B major") != 0 &&
						strcmp(value.data, "F# major") != 0 &&
						strcmp(value.data, "Gb major") != 0 &&
						strcmp(value.data, "Db major") != 0 &&
						strcmp(value.data, "Ab major") != 0 &&
						strcmp(value.data, "Eb major") != 0 &&
						strcmp(value.data, "Bb major") != 0 &&
						strcmp(value.data, "F major") != 0 &&
						strcmp(value.data, "Cb major") != 0 &&
						strcmp(value.data, "C# major") != 0 &&
						strcmp(value.data, "A minor") != 0 &&
						strcmp(value.data, "E minor") != 0 &&
						strcmp(value.data, "B minor") != 0 &&
						strcmp(value.data, "F# minor") != 0 &&
						strcmp(value.data, "C# minor") != 0 &&
						strcmp(value.data, "G# minor") != 0 &&
						strcmp(value.data, "D# minor") != 0 &&
						strcmp(value.data, "Eb minor") != 0 &&
						strcmp(value.data, "Bb minor") != 0 &&
						strcmp(value.data, "F minor") != 0 &&
						strcmp(value.data, "C minor") != 0 &&
						strcmp(value.data, "G minor") != 0 &&
						strcmp(value.data, "D minor") != 0)
					{
						printf("Error 0x%X: \tINVALID_KEY_SIG_PROVIDED '%s' at position %d, line %d, column %d.\n",
							LB_VALIDATION_INVALID_KEY_SIGNATURE_PROVIDED,
							value.data,
							read_position,
							line_position,
							column_position);
						validation_statuses |= LB_VALIDATION_INVALID_KEY_SIGNATURE_PROVIDED;
					}
				}
				else if (strcmp(header.data, "tempo") == 0)
				{
					tempo = atoi(value.data);
					if (tempo == 0)
					{
						if (strcmp(value.data, "largo") != 0 &&
							strcmp(value.data, "adagio") != 0 &&
							strcmp(value.data, "adante") != 0 &&
							strcmp(value.data, "moderato") != 0 &&
							strcmp(value.data, "allegro") != 0 &&
							strcmp(value.data, "presto") != 0)
						{
							printf("Error 0x%X: \tINVALID_TEMPO_PROVIDED '%s' at position %d, line %d, column %d.\n",
								LB_VALIDATION_INVALID_TEMPO_PROVIDED,
								value.data,
								read_position,
								line_position,
								column_position);
							validation_statuses |= LB_VALIDATION_INVALID_TEMPO_PROVIDED;
						}
					}
				}
				else if (strcmp(header.data, "timbre") == 0)
				{
					if (strcmp(value.data, "square wave") != 0 &&
						strcmp(value.data, "sine wave") != 0 &&
						strcmp(value.data, "triangle wave") != 0 &&
						strcmp(value.data, "sawtooth wave") != 0 &&
						strcmp(value.data, "pulse 10") != 0 &&
						strcmp(value.data, "pulse 25") != 0 &&
						strcmp(value.data, "noise") != 0)
					{
#ifdef _DEBUG
						lb_String filename = lb_new_string("../../../../../demos/data/samples/");
#else
						lb_String filename = lb_new_string("data/samples/");
#endif
						lb_String extension = lb_new_string(".pcm");
						strcat(filename.data, value.data);
						strcat(filename.data, extension.data);
						if (!exists(filename.data))
						{
							printf("Error 0x%X: \tINVALID_TIMBRE '%s' at position %d, line %d, column %d.\n",
								LB_VALIDATION_INVALID_TIMBRE,
								value.data,
								read_position,
								line_position,
								column_position);
							validation_statuses |= LB_VALIDATION_INVALID_TIMBRE;
						}
					}
				}
				else if (strcmp(header.data, "loop") == 0)
				{
					int loops = atoi(value.data);
					if (loops == 0)
					{
						if (strcmp(value.data, "none") != 0 &&
							strcmp(value.data, "infinity") != 0)
						{
							printf("Error 0x%X: \tINVALID_LOOP_VALUE '%s' at position %d, line %d, column %d.\n",
								LB_VALIDATION_INVALID_LOOP_VALUE,
								value.data,
								read_position,
								line_position,
								column_position);
							validation_statuses |= LB_VALIDATION_INVALID_LOOP_VALUE;
						}
					}
				}
				else if (strcmp(header.data, "dynamic") == 0)
				{
					if (strcmp(value.data, "ppp") != 0 &&
						strcmp(value.data, "pp") != 0 &&
						strcmp(value.data, "p") != 0 &&
						strcmp(value.data, "mp") != 0 &&
						strcmp(value.data, "mf") != 0 &&
						strcmp(value.data, "f") != 0 &&
						strcmp(value.data, "ff") != 0 &&
						strcmp(value.data, "fff") != 0)
					{
						printf("Error 0x%X: \tINVALID_DYNAMICS_VALUE '%s' at position %d, line %d, column %d.\n",
							LB_VALIDATION_INVALID_DYNAMICS_VALUE,
							value.data,
							read_position,
							line_position,
							column_position);
						validation_statuses |= LB_VALIDATION_INVALID_DYNAMICS_VALUE;
					}
				}
				else if (strcmp(header.data, "octave") == 0)
				{
					octave = atoi(value.data);
				}
				else if (strcmp(header.data, "panning") == 0)
				{
					float panning = atof(value.data);
					if (panning == 0)
					{
						if (strcmp(value.data, "far left") != 0 &&
							strcmp(value.data, "left") != 0 &&
							strcmp(value.data, "mono") != 0 &&
							strcmp(value.data, "right") != 0 &&
							strcmp(value.data, "far right") != 0)
						{
							printf("Error 0x%X: \tINVALID_PANNING_VALUE '%s' at position %d, line %d, column %d.\n",
								LB_VALIDATION_INVALID_PANNING_VALUE,
								value.data,
								read_position,
								line_position,
								column_position);
							validation_statuses |= LB_VALIDATION_INVALID_PANNING_VALUE;
						}
					}
					else if (panning < -1.0 && panning > 1.0)
					{
						printf("Error 0x%X: \tINVALID_PANNING_VALUE '%s' at position %d, line %d, column %d.\n",
							LB_VALIDATION_INVALID_PANNING_VALUE,
							value.data,
							read_position,
							line_position,
							column_position);
						validation_statuses |= LB_VALIDATION_INVALID_PANNING_VALUE;
					}
				}
				parse_state = previous_parse_state;
				unclosed_headers--;
				break;
			case '}':
				parse_state = LB_PARSE_STATE_READING_NOTHING;
				unclosed_track_scopes--;
				if (octave < 1 || octave > 7)
				{
					printf("Error 0x%X: \tOCTAVE_SHIFTS_OUT_OF_RANGE %d, instead of 1 to 7 non-inclusive, at position %d, line %d, column %d.\n",
						LB_VALIDATION_OCTAVE_SHIFTS_OUT_OF_RANGE,
						octave,
						read_position,
						line_position,
						column_position);
					validation_statuses |= LB_VALIDATION_OCTAVE_SHIFTS_OUT_OF_RANGE;
				}

				if (previous_bar_count != 0)
					if (current_bar_count != previous_bar_count)
					{
						printf("Error 0x%X: \tBAR_COUNTS_DO_NOT_MATCH %d != %d at position %d, line %d, column %d.\n",
							LB_VALIDATION_BAR_COUNTS_DO_NOT_MATCH,
							current_bar_count,
							previous_bar_count,
							read_position,
							line_position,
							column_position);
						validation_statuses |= LB_VALIDATION_BAR_COUNTS_DO_NOT_MATCH;
					}

				previous_bar_count = current_bar_count;
				current_bar_count = 0;
				break;
			case '+':
				octave++;
				break;
			case '-':
				octave--;
				break;
			default:
				if (symbol == '\n')
				{
					line_position++;
					column_position = 0;
				}
				else if (symbol == '\r')
				{
					column_position = 0;
				}

				if (parse_state == LB_PARSE_STATE_READING_HEADER)
				{
					lb_append_string(&header, script[read_position]);
				}
				else if (parse_state == LB_PARSE_STATE_READING_VALUE)
				{
					lb_append_string(&value, script[read_position]);
				}
				else if (parse_state == LB_PARSE_STATE_IGNORING_FIRST_SPACE_IN_VALUE)
				{
					printf("Error 0x%X: \tEXPECTED_SPACE_BETWEEN_HEADER_AND_VALUE at position %d, line %d, column %d.\n",
						LB_VALIDATION_EXPECTED_SPACE_BETWEEN_HEADER_AND_VALUE,
						read_position,
						line_position,
						column_position);
					validation_statuses |= LB_VALIDATION_EXPECTED_SPACE_BETWEEN_HEADER_AND_VALUE;
				}
				else if (parse_state == LB_PARSE_STATE_READING_TRACK_SCOPE)
				{
					if ((script[read_position] >= 'A' && script[read_position] <= 'G') ||
						script[read_position] == 'R')
						parse_state = LB_PARSE_STATE_READING_NOTE_FREQUENCY;
				}
				else if ((parse_state == LB_PARSE_STATE_READING_NOTE_FREQUENCY || parse_state == LB_PARSE_STATE_READING_NOTE_ACCIDENTAL) &&
					(script[read_position] >= '1' && script[read_position] <= '9'))
				{
					has_fractional_duration = false;
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
					if (has_fractional_duration)
						duration = 1.0 / atoi(duration_string.data);
					else
						duration = atoi(duration_string.data);

					duration += duration / 2.0;

					if (tuplet_is_unclosed)
						duration = (duration * 2.0) / 3.0;

					lb_clear_string(&duration_string);
					beats_in_a_bar += duration;
					parse_state = LB_PARSE_STATE_READING_TRACK_SCOPE;
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
				}
			}
		}

		if (parse_state == LB_PARSE_STATE_READING_NOTHING ||
			parse_state == LB_PARSE_STATE_READING_TRACK_SCOPE)
		{
			lb_clear_string(&header);
			lb_clear_string(&value);
		}

		read_position++;
		column_position++;
	} while (script[read_position] != 0);

	if (time_sig_lower == 0 || time_sig_upper == 0)
	{
		printf("Error 0x%X: \tINVALID_TIME_SIG_PROVIDED. %d/%d \n",
			time_sig_lower,
			time_sig_upper,
			LB_VALIDATION_INVALID_TIME_SIGNATURE_PROVIDED);
		validation_statuses |= LB_VALIDATION_INVALID_TIME_SIGNATURE_PROVIDED;
	}

	if (track_scope_count > MAX_TRACKS)
	{
		printf("Error 0x%X: \tTRACK_SCOPE_COUNT_EXCEEDS_MAXIMUM. %d > %d\n",
			track_scope_count,
			MAX_TRACKS,
			LB_VALIDATION_TRACK_SCOPE_COUNT_EXCEEDS_MAXIMUM);
		validation_statuses |= LB_VALIDATION_TRACK_SCOPE_COUNT_EXCEEDS_MAXIMUM;
	}

	else if (track_scope_count < 1)
	{
		printf("Error 0x%X: \tNO_TRACK_SCOPE_DETECTED -> '{ }'.\n",
			LB_VALIDATION_NO_TRACK_SCOPE_DETECTED);
		validation_statuses |= LB_VALIDATION_NO_TRACK_SCOPE_DETECTED;
	}

	if (unclosed_track_scopes > 0)
	{
		printf("Error 0x%X: \tUNCLOSED_TRACK_SCOPE -> '{'.\n",
			LB_VALIDATION_UNCLOSED_TRACK_SCOPE);
		validation_statuses |= LB_VALIDATION_UNCLOSED_TRACK_SCOPE;
	}

	else if (unclosed_track_scopes < 0)
	{
		printf("Error 0x%X: \tEXTRA_TRACK_SCOPE_CLOSED_BRACKET -> '}'.\n",
			LB_VALIDATION_EXTRA_TRACK_SCOPE_CLOSED_BRACKET);
		validation_statuses |= LB_VALIDATION_EXTRA_TRACK_SCOPE_CLOSED_BRACKET;
	}


	if (unclosed_headers > 0)
	{
		printf("Error 0x%X: \tUNCLOSED_HEADER_TAG -> '['.\n",
			LB_VALIDATION_UNCLOSED_HEADER_TAG);
		validation_statuses |= LB_VALIDATION_UNCLOSED_HEADER_TAG;
	}

	else if (unclosed_headers < 0)
	{
		printf("Error 0x%X: \tEXTRA_HEADER_TAG_CLOSED_BRACKET -> ']'.\n",
			LB_VALIDATION_EXTRA_HEADER_TAG_CLOSED_BRACKET);
		validation_statuses |= LB_VALIDATION_EXTRA_HEADER_TAG_CLOSED_BRACKET;
	}


	if (slur_is_unclosed)
	{
		printf("Error 0x%X: \tUNCLOSED_SLUR -> '~'.\n",
			LB_VALIDATION_UNCLOSED_SLUR);
		validation_statuses |= LB_VALIDATION_UNCLOSED_SLUR;
	}


	if (tuplet_is_unclosed)
	{
		printf("Error 0x%X: \tUNCLOSED_TUPLET -> '_'.\n",
			LB_VALIDATION_UNCLOSED_TUPLET);
		validation_statuses |= LB_VALIDATION_UNCLOSED_TUPLET;
	}


	if (unclosed_crescendos > 0)
	{
		printf("Error 0x%X: \tUNCLOSED_CRESCENDO -> '<<'.\n",
			LB_VALIDATION_UNCLOSED_CRESCENDO);
		validation_statuses |= LB_VALIDATION_UNCLOSED_CRESCENDO;
	}


	if (unclosed_diminuendos > 0)
	{
		printf("Error 0x%X: \tUNCLOSED_DIMINUENDO -> '>>'.\n",
			LB_VALIDATION_UNCLOSED_DIMINUENDO);
		validation_statuses |= LB_VALIDATION_UNCLOSED_DIMINUENDO;
	}

	if (validation_statuses == LB_VALIDATION_ALL_OK)
	{
		printf("No Errors: ALL_OK, Successfully passed validation.\n", LB_VALIDATION_ALL_OK);
	}
	else
	{
		printf("\n^Errors detected in parsing script.\n");
	}

	lb_free_string(&header);
	lb_free_string(&value);
	lb_free_string(&debug);
	lb_free_string(&duration_string);

	return validation_statuses;
}

bool validate_symbol(char symbol, uint8_t parse_state)
{
	bool is_valid = false;

	if (parse_state == LB_PARSE_STATE_READING_NOTHING)
	{
		if (symbol == '[' || symbol == '{' ||
			symbol == ' ' || symbol == '\n' ||
			symbol == '\r')
			is_valid = true;
	}
	else if (parse_state == LB_PARSE_STATE_READING_TRACK_SCOPE)
	{
		if (symbol >= 'A' && symbol <= 'G')
		{
			is_valid = true;
		}
		else
		{
			switch (symbol)
			{
			case '[':
			case ' ':
			case '\n':
			case '\r':
			case '|':
			case '~':
			case '+':
			case '-':
			case '_':
			case '<':
			case '>':
			case 'R':
			case '}':
				is_valid = true;
			}
		}
	}
	else if (parse_state == LB_PARSE_STATE_READING_NOTE_FREQUENCY)
	{
		if (symbol >= '1' && symbol <= '9')
		{
			is_valid = true;
		}
		else
		{
			switch (symbol)
			{
			case '#': case 'b':	case 'n': case '/':
				is_valid = true;
			}
		}
	}
	else if (parse_state == LB_PARSE_STATE_READING_NOTE_ACCIDENTAL)
	{
		if (symbol >= '1' && symbol <= '9')
			is_valid = true;
	}
	else if (parse_state == LB_PARSE_STATE_READING_NOTE_DURATION)
	{
		if (symbol >= '0' && symbol <= '9')
		{
			is_valid = true;
		}
		else
		{
			switch (symbol)
			{
			case '*':
			case '.':
			case '>':
			case ' ':
				is_valid = true;
			}
		}
	}
	else
	{
		is_valid = true;
	}

	return is_valid;
}