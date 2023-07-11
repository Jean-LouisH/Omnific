#include "include/ScriptValidator.h"
#include "include/Validation.h"
#include "include/ScriptParseStates.h"
#include "include/Constants.h"
#include "include/Strings.h"
#include "include/File.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int validateScript(char* script)
{
	int validationStatuses = LB_VALIDATION_ALL_OK;
	int readPosition = 0;
	int linePosition = 1;
	int columnPosition = 1;
	int trackScopeCount = 0;
	int unclosedHeaders = 0;
	int unclosedTrackScopes = 0;
	uint8_t timeSigLower = 0;
	uint8_t timeSigUpper = 0;
	double beatsInABar = 0;
	unsigned int previousBarCount = 0;
	unsigned int currentBarCount = 0;
	uint8_t octave = 0;
	uint8_t tempo = 0;
	uint8_t parseState = LB_PARSE_STATE_READING_NOTHING;
	uint8_t previousParseState = 0;

	bool tupletIsUnclosed = false;
	bool slurIsUnclosed = false;

	bool isReadingCrescendo = false;
	bool isReadingDiminuendo = false;
	int unclosedCrescendos = 0;
	int unclosedDiminuendos = 0;

	lb_String header = lb_newString("");
	lb_String value = lb_newString("");
	lb_String debug = lb_newString("");

	double duration = 0.0;
	bool hasFractionalDuration = false;
	lb_String durationString = lb_newString("");

	printf("\n\t\tLibretti Script Validation Statuses");
	printf("\n\n");

	do
	{
		char symbol = script[readPosition];

		lb_appendString(&debug, symbol);
		if (!validateSymbol(symbol, parseState))
		{
			printf("Error 0x%X: \tINVALID_USE_OF_SYMBOL '%c' at position %d, line %d, column %d.\n", 
				LB_VALIDATION_INVALID_USE_OF_SYMBOL, 
				symbol,
				readPosition,
				linePosition,
				columnPosition);
			validationStatuses |= LB_VALIDATION_INVALID_USE_OF_SYMBOL;
		}
		else
		{
			switch (symbol)
			{
			case '{':
				parseState = LB_PARSE_STATE_READING_TRACK_SCOPE;
				trackScopeCount++;
				unclosedTrackScopes++;
				break;
			case '[':
				previousParseState = parseState;
				parseState = LB_PARSE_STATE_READING_HEADER;
				unclosedHeaders++;
				break;
			case '_':
				tupletIsUnclosed = !tupletIsUnclosed;
				break;
			case '~':
				slurIsUnclosed = !slurIsUnclosed;
				break;
			case '<':
				if (!isReadingCrescendo)
				{
					isReadingCrescendo = true;
				}
				else
				{
					if (unclosedCrescendos > 0)
						unclosedCrescendos--;
					else
						unclosedCrescendos++;
					isReadingCrescendo = false;
				}
				break;
			case '>':
				if (!isReadingDiminuendo)
				{
					isReadingDiminuendo = true;
				}
				else
				{
					if (unclosedDiminuendos > 0)
						unclosedDiminuendos--;
					else
						unclosedDiminuendos++;
					isReadingDiminuendo = false;
				}
				break;
			case ' ':
				if (isReadingCrescendo)
					isReadingCrescendo = false;

				if (isReadingDiminuendo)
					isReadingDiminuendo = false;

				if (parseState == LB_PARSE_STATE_IGNORING_FIRST_SPACE_IN_VALUE)
				{
					parseState = LB_PARSE_STATE_READING_VALUE;
				}
				else if (parseState == LB_PARSE_STATE_READING_HEADER)
				{
					lb_appendString(&header, script[readPosition]);
				}
				else if (parseState == LB_PARSE_STATE_READING_VALUE)
				{
					lb_appendString(&value, script[readPosition]);
				}
				else if (parseState == LB_PARSE_STATE_READING_NOTE_DURATION)
				{
					if (hasFractionalDuration)
						duration = 1.0 / atoi(durationString.data);
					else
						duration = atoi(durationString.data);

					if (tupletIsUnclosed)
						duration = (duration * 2.0) / 3.0;

					lb_clearString(&durationString);
					beatsInABar += duration;
					parseState = LB_PARSE_STATE_READING_TRACK_SCOPE;
				}
				break;
			case '|':
				if (beatsInABar != 0)
					if (round(beatsInABar) != timeSigUpper)
					{
						printf("Error 0x%X: \tBEATS_DO_NOT_MATCH_TIME_SIG %d != %d at position %d, line %d, column %d.\n",
							LB_VALIDATION_BEATS_DO_NOT_MATCH_TIME_SIG,
							beatsInABar,
							timeSigUpper,
							readPosition,
							linePosition,
							columnPosition);
						validationStatuses |= LB_VALIDATION_BEATS_DO_NOT_MATCH_TIME_SIG;
					}
				beatsInABar = 0;
				currentBarCount++;
				break;
			case ':':
				parseState = LB_PARSE_STATE_IGNORING_FIRST_SPACE_IN_VALUE;
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
						readPosition,
						linePosition,
						columnPosition);
					validationStatuses |= LB_VALIDATION_UNEXPECTED_HEADER_NAME;
				}
				break;
			case ']':
				if (strcmp(header.data, "time signature") == 0)
				{
					int valueReadPosition = 0;
					lb_String upper = lb_newString("");
					lb_String lower = lb_newString("");

					while (value.data[valueReadPosition] != 0 &&
						value.data[valueReadPosition] != '/')
					{
						lb_appendString(&upper, value.data[valueReadPosition]);
						valueReadPosition++;
					}

					valueReadPosition++;

					while (value.data[valueReadPosition] != 0)
					{
						lb_appendString(&lower, value.data[valueReadPosition]);
						valueReadPosition++;
					}

					timeSigUpper = atoi(upper.data);
					timeSigLower = atoi(lower.data);
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
							readPosition,
							linePosition,
							columnPosition);
						validationStatuses |= LB_VALIDATION_INVALID_KEY_SIGNATURE_PROVIDED;
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
								readPosition,
								linePosition,
								columnPosition);
							validationStatuses |= LB_VALIDATION_INVALID_TEMPO_PROVIDED;
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
						lb_String filename = lb_newString("../../../../../demos/data/samples/");
#else
						lb_String filename = lb_newString("data/samples/");
#endif
						lb_String extension = lb_newString(".pcm");
						strcat(filename.data, value.data);
						strcat(filename.data, extension.data);
						if (!exists(filename.data))
						{
							printf("Error 0x%X: \tINVALID_TIMBRE '%s' at position %d, line %d, column %d.\n",
								LB_VALIDATION_INVALID_TIMBRE,
								value.data,
								readPosition,
								linePosition,
								columnPosition);
							validationStatuses |= LB_VALIDATION_INVALID_TIMBRE;
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
								readPosition,
								linePosition,
								columnPosition);
							validationStatuses |= LB_VALIDATION_INVALID_LOOP_VALUE;
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
							readPosition,
							linePosition,
							columnPosition);
						validationStatuses |= LB_VALIDATION_INVALID_DYNAMICS_VALUE;
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
								readPosition,
								linePosition,
								columnPosition);
							validationStatuses |= LB_VALIDATION_INVALID_PANNING_VALUE;
						}
					}
					else if (panning < -1.0 && panning > 1.0)
					{
						printf("Error 0x%X: \tINVALID_PANNING_VALUE '%s' at position %d, line %d, column %d.\n",
							LB_VALIDATION_INVALID_PANNING_VALUE,
							value.data,
							readPosition,
							linePosition,
							columnPosition);
						validationStatuses |= LB_VALIDATION_INVALID_PANNING_VALUE;
					}
				}
				parseState = previousParseState;
				unclosedHeaders--;
				break;
			case '}':
				parseState = LB_PARSE_STATE_READING_NOTHING;
				unclosedTrackScopes--;
				if (octave < 1 || octave > 7)
				{
					printf("Error 0x%X: \tOCTAVE_SHIFTS_OUT_OF_RANGE %d, instead of 1 to 7 non-inclusive, at position %d, line %d, column %d.\n",
						LB_VALIDATION_OCTAVE_SHIFTS_OUT_OF_RANGE,
						octave,
						readPosition,
						linePosition,
						columnPosition);
					validationStatuses |= LB_VALIDATION_OCTAVE_SHIFTS_OUT_OF_RANGE;
				}

				if (previousBarCount != 0)
					if (currentBarCount != previousBarCount)
					{
						printf("Error 0x%X: \tBAR_COUNTS_DO_NOT_MATCH %d != %d at position %d, line %d, column %d.\n",
							LB_VALIDATION_BAR_COUNTS_DO_NOT_MATCH,
							currentBarCount,
							previousBarCount,
							readPosition,
							linePosition,
							columnPosition);
						validationStatuses |= LB_VALIDATION_BAR_COUNTS_DO_NOT_MATCH;
					}

				previousBarCount = currentBarCount;
				currentBarCount = 0;
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
					linePosition++;
					columnPosition = 0;
				}
				else if (symbol == '\r')
				{
					columnPosition = 0;
				}

				if (parseState == LB_PARSE_STATE_READING_HEADER)
				{
					lb_appendString(&header, script[readPosition]);
				}
				else if (parseState == LB_PARSE_STATE_READING_VALUE)
				{
					lb_appendString(&value, script[readPosition]);
				}
				else if (parseState == LB_PARSE_STATE_IGNORING_FIRST_SPACE_IN_VALUE)
				{
					printf("Error 0x%X: \tEXPECTED_SPACE_BETWEEN_HEADER_AND_VALUE at position %d, line %d, column %d.\n",
						LB_VALIDATION_EXPECTED_SPACE_BETWEEN_HEADER_AND_VALUE,
						readPosition,
						linePosition,
						columnPosition);
					validationStatuses |= LB_VALIDATION_EXPECTED_SPACE_BETWEEN_HEADER_AND_VALUE;
				}
				else if (parseState == LB_PARSE_STATE_READING_TRACK_SCOPE)
				{
					if ((script[readPosition] >= 'A' && script[readPosition] <= 'G') ||
						script[readPosition] == 'R')
						parseState = LB_PARSE_STATE_READING_NOTE_FREQUENCY;
				}
				else if ((parseState == LB_PARSE_STATE_READING_NOTE_FREQUENCY || parseState == LB_PARSE_STATE_READING_NOTE_ACCIDENTAL) &&
					(script[readPosition] >= '1' && script[readPosition] <= '9'))
				{
					hasFractionalDuration = false;
					parseState = LB_PARSE_STATE_READING_NOTE_DURATION;
					lb_appendString(&durationString, script[readPosition]);
				}
				else if ((parseState == LB_PARSE_STATE_READING_NOTE_FREQUENCY || parseState == LB_PARSE_STATE_READING_NOTE_ACCIDENTAL) &&
					(script[readPosition] == '/'))
				{
					hasFractionalDuration = true;
					parseState = LB_PARSE_STATE_READING_NOTE_DURATION;
				}
				else if (parseState == LB_PARSE_STATE_READING_NOTE_DURATION && script[readPosition] == '.')
				{
					if (hasFractionalDuration)
						duration = 1.0 / atoi(durationString.data);
					else
						duration = atoi(durationString.data);

					duration += duration / 2.0;

					if (tupletIsUnclosed)
						duration = (duration * 2.0) / 3.0;

					lb_clearString(&durationString);
					beatsInABar += duration;
					parseState = LB_PARSE_STATE_READING_TRACK_SCOPE;
				}
				else if (parseState == LB_PARSE_STATE_READING_NOTE_DURATION &&
					(script[readPosition] >= '0' && script[readPosition] <= '9'))
				{
					lb_appendString(&durationString, script[readPosition]);
				}
				else if (parseState == LB_PARSE_STATE_READING_NOTE_FREQUENCY &&
					(script[readPosition] == '#' || script[readPosition] == 'b' ||
						script[readPosition] == 'n'))
				{
					parseState = LB_PARSE_STATE_READING_NOTE_ACCIDENTAL;
				}
			}
		}

		if (parseState == LB_PARSE_STATE_READING_NOTHING ||
			parseState == LB_PARSE_STATE_READING_TRACK_SCOPE)
		{
			lb_clearString(&header);
			lb_clearString(&value);
		}

		readPosition++;
		columnPosition++;
	} while (script[readPosition] != NULL);

	if (timeSigLower == 0 || timeSigUpper == 0)
	{
		printf("Error 0x%X: \tINVALID_TIME_SIG_PROVIDED. %d/%d \n",
			timeSigLower,
			timeSigUpper,
			LB_VALIDATION_INVALID_TIME_SIGNATURE_PROVIDED);
		validationStatuses |= LB_VALIDATION_INVALID_TIME_SIGNATURE_PROVIDED;
	}

	if (trackScopeCount > MAX_TRACKS)
	{
		printf("Error 0x%X: \tTRACK_SCOPE_COUNT_EXCEEDS_MAXIMUM. %d > %d\n",
			trackScopeCount,
			MAX_TRACKS,
			LB_VALIDATION_TRACK_SCOPE_COUNT_EXCEEDS_MAXIMUM);
		validationStatuses |= LB_VALIDATION_TRACK_SCOPE_COUNT_EXCEEDS_MAXIMUM;
	}

	else if (trackScopeCount < 1)
	{
		printf("Error 0x%X: \tNO_TRACK_SCOPE_DETECTED -> '{ }'.\n",
			LB_VALIDATION_NO_TRACK_SCOPE_DETECTED);
		validationStatuses |= LB_VALIDATION_NO_TRACK_SCOPE_DETECTED;
	}

	if (unclosedTrackScopes > 0)
	{
		printf("Error 0x%X: \tUNCLOSED_TRACK_SCOPE -> '{'.\n",
			LB_VALIDATION_UNCLOSED_TRACK_SCOPE);
		validationStatuses |= LB_VALIDATION_UNCLOSED_TRACK_SCOPE;
	}

	else if (unclosedTrackScopes < 0)
	{
		printf("Error 0x%X: \tEXTRA_TRACK_SCOPE_CLOSED_BRACKET -> '}'.\n",
			LB_VALIDATION_EXTRA_TRACK_SCOPE_CLOSED_BRACKET);
		validationStatuses |= LB_VALIDATION_EXTRA_TRACK_SCOPE_CLOSED_BRACKET;
	}


	if (unclosedHeaders > 0)
	{
		printf("Error 0x%X: \tUNCLOSED_HEADER_TAG -> '['.\n",
			LB_VALIDATION_UNCLOSED_HEADER_TAG);
		validationStatuses |= LB_VALIDATION_UNCLOSED_HEADER_TAG;
	}

	else if (unclosedHeaders < 0)
	{
		printf("Error 0x%X: \tEXTRA_HEADER_TAG_CLOSED_BRACKET -> ']'.\n",
			LB_VALIDATION_EXTRA_HEADER_TAG_CLOSED_BRACKET);
		validationStatuses |= LB_VALIDATION_EXTRA_HEADER_TAG_CLOSED_BRACKET;
	}


	if (slurIsUnclosed)
	{
		printf("Error 0x%X: \tUNCLOSED_SLUR -> '~'.\n",
			LB_VALIDATION_UNCLOSED_SLUR);
		validationStatuses |= LB_VALIDATION_UNCLOSED_SLUR;
	}


	if (tupletIsUnclosed)
	{
		printf("Error 0x%X: \tUNCLOSED_TUPLET -> '_'.\n",
			LB_VALIDATION_UNCLOSED_TUPLET);
		validationStatuses |= LB_VALIDATION_UNCLOSED_TUPLET;
	}


	if (unclosedCrescendos > 0)
	{
		printf("Error 0x%X: \tUNCLOSED_CRESCENDO -> '<<'.\n",
			LB_VALIDATION_UNCLOSED_CRESCENDO);
		validationStatuses |= LB_VALIDATION_UNCLOSED_CRESCENDO;
	}


	if (unclosedDiminuendos > 0)
	{
		printf("Error 0x%X: \tUNCLOSED_DIMINUENDO -> '>>'.\n",
			LB_VALIDATION_UNCLOSED_DIMINUENDO);
		validationStatuses |= LB_VALIDATION_UNCLOSED_DIMINUENDO;
	}

	if (validationStatuses == LB_VALIDATION_ALL_OK)
	{
		printf("No Errors: ALL_OK, Successfully passed validation.\n", LB_VALIDATION_ALL_OK);
	}
	else
	{
		printf("\n^Errors detected in parsing script.\n");
	}

	lb_freeString(&header);
	lb_freeString(&value);
	lb_freeString(&debug);
	lb_freeString(&durationString);

	return validationStatuses;
}

bool validateSymbol(char symbol, uint8_t parseState)
{
	bool isValid = false;

	if (parseState == LB_PARSE_STATE_READING_NOTHING)
	{
		if (symbol == '[' || symbol == '{' ||
			symbol == ' ' || symbol == '\n' ||
			symbol == '\r')
			isValid = true;
	}
	else if (parseState == LB_PARSE_STATE_READING_TRACK_SCOPE)
	{
		if (symbol >= 'A' && symbol <= 'G')
		{
			isValid = true;
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
				isValid = true;
			}
		}
	}
	else if (parseState == LB_PARSE_STATE_READING_NOTE_FREQUENCY)
	{
		if (symbol >= '1' && symbol <= '9')
		{
			isValid = true;
		}
		else
		{
			switch (symbol)
			{
			case '#': case 'b':	case 'n': case '/':
				isValid = true;
			}
		}
	}
	else if (parseState == LB_PARSE_STATE_READING_NOTE_ACCIDENTAL)
	{
		if (symbol >= '1' && symbol <= '9')
			isValid = true;
	}
	else if (parseState == LB_PARSE_STATE_READING_NOTE_DURATION)
	{
		if (symbol >= '0' && symbol <= '9')
		{
			isValid = true;
		}
		else
		{
			switch (symbol)
			{
			case '*':
			case '.':
			case '>':
			case ' ':
				isValid = true;
			}
		}
	}
	else
	{
		isValid = true;
	}

	return isValid;
}