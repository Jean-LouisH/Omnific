#include "include/Compiler.h"
#include "include/File.h"
#include "include/ScriptParseStates.h"
#include "include/ScriptValidator.h"
#include "include/Validation.h"
#include "include/Strings.h"
#include "include/Timing.h"
#include "include/Notes.h"
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

void compileCompositionFromScript(lb_Composition* composition, const char* script)
{
	composition->validationStatuses = validateScript(script);

	if (composition->validationStatuses == LB_VALIDATION_ALL_OK)
	{
		allocateMemory(composition, script);
		buildCompositionData(composition, script);
	}
	else
	{
		printf("\nCompilation Failed.");
	}
}

void allocateMemory(lb_Composition* composition, const char* script)
{
	int readPosition = 0;
	int currentTrack = -1;
	int noteCount = 0;
	int tempoEventCount = 0;
	int lyricsEventCount = 0;
	char symbol = NULL;
	bool isReadingHeader = false;
	bool isReadingValue = false;
	lb_String header = lb_newString("");

	do
	{
		symbol = script[readPosition];

		switch (symbol)
		{
		case '{':
			currentTrack++;
			noteCount = 0;
			break;
		case '[':
			isReadingHeader = true;
			break;
		case ':':
			if (strcmp(header.data, "tempo") == 0)
				tempoEventCount++;
			else if (strcmp(header.data, "lyric") == 0)
				lyricsEventCount++;
			isReadingHeader = false;
			isReadingValue = true;
			lb_clearString(&header);
			break;
		case '}':
			if (noteCount > 0)
				composition->tracks[currentTrack].noteEvents = malloc(sizeof(lb_NoteEvent) * noteCount);
			else
				composition->tracks[currentTrack].noteEvents = calloc(1, sizeof(lb_NoteEvent));
			composition->tracks[currentTrack].noteCount = noteCount;
			break;
		case ']':
			isReadingValue = false;
			break;
		default:
			if (!isReadingValue)
			{
				if (isReadingHeader)
					lb_appendString(&header, symbol);
				else if (symbol >= 'A' && symbol <= 'G' ||
					symbol == 'R')
					noteCount++;
			}
		}
		readPosition++;
	} while (symbol != NULL);

	composition->tempoEvents = malloc(sizeof(lb_TempoEvent) * tempoEventCount);
	composition->tempoEventCount = tempoEventCount;
	composition->lyricsEvents = malloc(sizeof(lb_LyricsEvent) * lyricsEventCount);
	composition->lyricsEventCount = lyricsEventCount;
	composition->trackCount = currentTrack + 1;
	lb_freeString(&header);
}

void buildCompositionData(lb_Composition* composition, const char* script)
{
	uint32_t readPosition = 0;
	uint32_t currentNote = 0;
	uint8_t currentTrack = 0;
	uint32_t currentTempoEvent = 0;
	uint32_t currentLyricsEvent = 0;

	double currentTime_s = 0.0;
	uint16_t barCount = 0;

	uint8_t timeSigLower = 0;
	uint8_t timeSigUpper = 0;
	uint8_t octave = 0;
	uint16_t tempo = 0;
	uint16_t dynamic = 0;
	int8_t panning = 0;
	uint8_t timbre = 0;
	uint16_t cue = 0;
	double duration = 0.0;
	bool tupletIsOpened = false;
	bool slurIsOpened = false;
	bool isReadingCrescendo = false;
	bool isReadingDiminuendo = false;
	bool hasFractionalDuration = false;
	bool hasDottedDuration = false;

	bool isInCrescendoState = false;
	bool isInDiminuendoState = false;
	uint32_t gradualDynamicsNoteIndices[256] = {0};
	uint16_t gradualDynamicsNoteCount = 0;

	lb_Note note;
	lb_Effects effects;
	effects.echo.decay = 0.0;
	effects.echo.delay = 0.0;
	effects.reverb.damping = 0.0;
	effects.reverb.preDelay = 0.0;
	effects.reverb.roomSize = 0.0;
	effects.vibrato.extent = 0.0;
	effects.vibrato.rate = 0.0;
	effects.crossfading = 0.0;
	effects.pitchBlend = 0.0;

	for (int i = 0; i < 10; i++)
		effects.eq[i].level = 0;

	char noteToPlay;

	uint8_t parseState = LB_PARSE_STATE_READING_NOTHING;
	uint8_t previousParseState = 0;
	lb_String header = lb_newString("");
	lb_String value = lb_newString("");

	lb_String durationString = lb_newString("");

	do
	{
		switch (script[readPosition])
		{
		case '{':
			parseState = LB_PARSE_STATE_READING_TRACK_SCOPE;
			break;
		case '[':
			previousParseState = parseState;
			parseState = LB_PARSE_STATE_READING_HEADER;
			break;
		case '_':
			tupletIsOpened = !tupletIsOpened;
			break;
		case '~':
			slurIsOpened = !slurIsOpened;
			break;
		case '<':
			if (isReadingCrescendo)
			{
				isInCrescendoState = !isInCrescendoState;

				if (isInCrescendoState)
					gradualDynamicsNoteCount = 0;
			}

			isReadingCrescendo = !isReadingCrescendo;
			break;
		case '>':
			if (parseState == LB_PARSE_STATE_READING_NOTE_DURATION)
			{
				parseState = LB_PARSE_STATE_READING_DYNAMIC_ACCENT;
			}
			else
			{
				if (isReadingDiminuendo)
				{
					isInDiminuendoState = !isInDiminuendoState;

					if (isInDiminuendoState)
						gradualDynamicsNoteCount = 0;
				}

				isReadingDiminuendo = !isReadingDiminuendo;
			}
			break;
		case '*':
			if (parseState == LB_PARSE_STATE_READING_NOTE_DURATION)
				parseState = LB_PARSE_STATE_READING_STACCATO;
			break;
		case '|':
			barCount++;
			duration = (double)timeSigUpper;
			duration *= (60.0 / (double)tempo) / (4.0 / (double)timeSigLower);
			currentTime_s = duration * barCount;
			break;
		case ' ':
			if (isReadingCrescendo)
				isReadingCrescendo = false;

			if (isReadingDiminuendo)
				isReadingDiminuendo = false;

			if (parseState == LB_PARSE_STATE_READING_NOTE_DURATION ||
				parseState == LB_PARSE_STATE_READING_DYNAMIC_ACCENT ||
				parseState == LB_PARSE_STATE_READING_STACCATO)
			{
				if (parseState == LB_PARSE_STATE_READING_DYNAMIC_ACCENT)
					note.dynamic = dynamic * 2;
				else
					note.dynamic = dynamic;

				if (parseState == LB_PARSE_STATE_READING_STACCATO)
					note.articulation = LB_ARTICULATION_STACCATO;
				else if (slurIsOpened)
					note.articulation = LB_ARTICULATION_SLUR;
				else
					note.articulation = LB_ARTICULATION_NORMAL;

				note.cue = cue;
				note.panning = panning;
				note.timbre = timbre;
				note.effects = effects;

				composition->tracks[currentTrack].noteEvents[currentNote].note = note;
				composition->tracks[currentTrack].noteEvents[currentNote].startTime = currentTime_s;

				if (isInCrescendoState || isInDiminuendoState)
				{
					gradualDynamicsNoteIndices[gradualDynamicsNoteCount] = currentNote;
					gradualDynamicsNoteCount++;
				}

				if (hasFractionalDuration)
					duration = 1.0 / atoi(durationString.data);
				else if (hasDottedDuration)
					;
				else
					duration = atoi(durationString.data);

				lb_clearString(&durationString);

				if (tupletIsOpened)
					duration = (duration * 2.0) / 3.0;

				double secondsPerBeat = 60.0 / (double)tempo;
				double beatLength = 4.0 / (double)timeSigLower;
				secondsPerBeat /= beatLength;
				duration *= secondsPerBeat;
				currentTime_s += duration;
				parseState = LB_PARSE_STATE_READING_TRACK_SCOPE;
				currentNote++;
			}

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
			break;
		case ':':
			parseState = LB_PARSE_STATE_IGNORING_FIRST_SPACE_IN_VALUE;
			break;
		case ']':
			if (strcmp(header.data, "name") == 0)
			{
				int valueReadPosition = 0;
				while (value.data[valueReadPosition] != 0)
				{
					composition->name[valueReadPosition] = value.data[valueReadPosition];
					valueReadPosition++;
				}
			}
			else if (strcmp(header.data, "artist") == 0)
			{
				int valueReadPosition = 0;
				while (value.data[valueReadPosition] != 0)
				{
					composition->artist[valueReadPosition] = value.data[valueReadPosition];
					valueReadPosition++;
				}
			}
			else if (strcmp(header.data, "key signature") == 0)
			{
				if (strcmp(value.data, "C major") == 0)
					composition->keySignature = LB_KEY_SIGNATURE_C_MAJOR;
				else if (strcmp(value.data, "G major") == 0)
					composition->keySignature = LB_KEY_SIGNATURE_G_MAJOR;
				else if (strcmp(value.data, "D major") == 0)
					composition->keySignature = LB_KEY_SIGNATURE_D_MAJOR;
				else if (strcmp(value.data, "A major") == 0)
					composition->keySignature = LB_KEY_SIGNATURE_A_MAJOR;
				else if (strcmp(value.data, "E major") == 0)
					composition->keySignature = LB_KEY_SIGNATURE_E_MAJOR;
				else if (strcmp(value.data, "B major") == 0)
					composition->keySignature = LB_KEY_SIGNATURE_B_MAJOR;
				else if (strcmp(value.data, "F# major") == 0)
					composition->keySignature = LB_KEY_SIGNATURE_Fs_MAJOR;
				else if (strcmp(value.data, "Gb major") == 0)
					composition->keySignature = LB_KEY_SIGNATURE_Gb_MAJOR;
				else if (strcmp(value.data, "Db major") == 0)
					composition->keySignature = LB_KEY_SIGNATURE_Db_MAJOR;
				else if (strcmp(value.data, "Ab major") == 0)
					composition->keySignature = LB_KEY_SIGNATURE_Ab_MAJOR;
				else if (strcmp(value.data, "Eb major") == 0)
					composition->keySignature = LB_KEY_SIGNATURE_Eb_MAJOR;
				else if (strcmp(value.data, "Bb major") == 0)
					composition->keySignature = LB_KEY_SIGNATURE_Bb_MAJOR;
				else if (strcmp(value.data, "F major") == 0)
					composition->keySignature = LB_KEY_SIGNATURE_F_MAJOR;
				else if (strcmp(value.data, "Cb major") == 0)
					composition->keySignature = LB_KEY_SIGNATURE_Cb_MAJOR;
				else if (strcmp(value.data, "C# major") == 0)
					composition->keySignature = LB_KEY_SIGNATURE_Cs_MAJOR;
				else if (strcmp(value.data, "A minor") == 0)
					composition->keySignature = LB_KEY_SIGNATURE_A_MINOR;
				else if (strcmp(value.data, "E minor") == 0)
					composition->keySignature = LB_KEY_SIGNATURE_E_MINOR;
				else if (strcmp(value.data, "B minor") == 0)
					composition->keySignature = LB_KEY_SIGNATURE_B_MINOR;
				else if (strcmp(value.data, "F# minor") == 0)
					composition->keySignature = LB_KEY_SIGNATURE_Fs_MINOR;
				else if (strcmp(value.data, "C# minor") == 0)
					composition->keySignature = LB_KEY_SIGNATURE_Cs_MINOR;
				else if (strcmp(value.data, "G# minor") == 0)
					composition->keySignature = LB_KEY_SIGNATURE_Gs_MINOR;
				else if (strcmp(value.data, "D# minor") == 0)
					composition->keySignature = LB_KEY_SIGNATURE_Ds_MINOR;
				else if (strcmp(value.data, "Eb minor") == 0)
					composition->keySignature = LB_KEY_SIGNATURE_Eb_MINOR;
				else if (strcmp(value.data, "Bb minor") == 0)
					composition->keySignature = LB_KEY_SIGNATURE_Bb_MINOR;
				else if (strcmp(value.data, "F minor") == 0)
					composition->keySignature = LB_KEY_SIGNATURE_F_MINOR;
				else if (strcmp(value.data, "C minor") == 0)
					composition->keySignature = LB_KEY_SIGNATURE_C_MINOR;
				else if (strcmp(value.data, "G minor") == 0)
					composition->keySignature = LB_KEY_SIGNATURE_G_MINOR;
				else if (strcmp(value.data, "D minor") == 0)
					composition->keySignature = LB_KEY_SIGNATURE_D_MINOR;
			}
			else if (strcmp(header.data, "time signature") == 0)
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

				composition->timeSignature[0] = timeSigUpper;
				composition->timeSignature[1] = timeSigLower;

				lb_freeString(&upper);
				lb_freeString(&lower);
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
				composition->tempoEvents[currentTempoEvent].tempo = tempo;
				composition->tempoEvents[currentTempoEvent].startTime = currentTime_s;
				currentTempoEvent++;
			}
			else if (strcmp(header.data, "dynamic") == 0)
			{
				uint16_t previousDynamic = dynamic;

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


				if (!isInCrescendoState && !isInDiminuendoState)
				{
					for (int i = 0; i < gradualDynamicsNoteCount; i++)
					{
						uint32_t noteIndex = gradualDynamicsNoteIndices[i];
						float gradualDynamicDelta = (float)(dynamic - previousDynamic) / (float)gradualDynamicsNoteCount;
						uint16_t gradualDynamic = (uint16_t)((float)previousDynamic + (gradualDynamicDelta * i));
						composition->tracks[currentTrack].noteEvents[noteIndex].note.dynamic = gradualDynamic;
					}

					gradualDynamicsNoteCount = 0;
				}

			}
			else if (strcmp(header.data, "reverb") == 0)
			{
				float reverbValue = atof(value.data);
				if (reverbValue == 0)
				{
					if (strcmp(value.data, "standard") == 0)
					{
						effects.reverb.damping = 50;
						effects.reverb.preDelay = 10;
						effects.reverb.roomSize = 50;
					}
					else if (strcmp(value.data, "none") == 0)
					{
						effects.reverb.damping = 0;
						effects.reverb.preDelay = 0;
						effects.reverb.roomSize = 0;
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
				float panningValue = atof(value.data);
				if (panningValue == 0)
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
					panning = panningValue * 127;
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
					lb_String filename = lb_newString("../../../../../demos/data/samples/");
#else
					lb_String filename = lb_newString("data/samples/");
#endif
					lb_String extension = lb_newString(".pcm");
					timbre = LB_TIMBRE_SAMPLE;
					strcat(filename.data, value.data);
					strcat(filename.data, extension.data);
					lb_BinaryS16 binary = loadBinaryS16FromFile(filename.data);
					float sampleProgressDelta = (float)binary.size / (float)SAMPLE_SIZE;

					int16_t debugBinaryData[2048] = { 0 };

					for (int i = 0; i < binary.size; i++)
						debugBinaryData[i] = binary.data[i];

					for (int i = 0; i < SAMPLE_SIZE; i++)
						note.sample[i] = binary.data[(int)(sampleProgressDelta * i)];

					lb_freeString(&filename);
					lb_freeString(&extension);
				}
			}
			else if (strcmp(header.data, "octave") == 0)
			{
				octave = atoi(value.data);
			}
			else if (strcmp(header.data, "loop") == 0)
			{
				composition->loopTimestamp = currentTime_s;
				composition->loopCount = atoi(value.data);
				if (strcmp(value.data, "infinity") == 0 || 
					composition->loopCount > pow(2, (sizeof composition->loopCount) * 8) - 1)
					composition->loopCount = pow(2, (sizeof composition->loopCount) * 8) - 1;
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
				//audio->lyricsEvents[currentLyricsEvent].lyrics = malloc(sizeof(char) * value.capacity);
				strcpy(composition->lyricsEvents[currentLyricsEvent].lyrics, value.data);
				composition->lyricsEvents[currentLyricsEvent].startTime = currentTime_s;
				currentLyricsEvent++;
			}
			else if (strcmp(header.data, "eq") == 0)
			{
				int valueReadPosition = 0;
				lb_String frequencyString = lb_newString("");
				lb_String levelString = lb_newString("");

				while (value.data[valueReadPosition] != 0 &&
					value.data[valueReadPosition] != ';')
				{
					lb_appendString(&frequencyString, value.data[valueReadPosition]);
					valueReadPosition++;
				}

				valueReadPosition++;

				while (value.data[valueReadPosition] != 0)
				{
					lb_appendString(&levelString, value.data[valueReadPosition]);
					valueReadPosition++;
				}

				if (strcmp(value.data, "none") == 0)
				{
					for (int i = 0; i < 10; i++)
						effects.eq[i].level = 0;
				}
				else
				{
					if (strcmp(frequencyString.data, "31Hz") == 0)
						effects.eq[0].level = atof(levelString.data);
					if (strcmp(frequencyString.data, "62Hz") == 0)
						effects.eq[1].level = atof(levelString.data);
					if (strcmp(frequencyString.data, "125Hz") == 0)
						effects.eq[2].level = atof(levelString.data);
					if (strcmp(frequencyString.data, "250Hz") == 0)
						effects.eq[3].level = atof(levelString.data);
					if (strcmp(frequencyString.data, "500Hz") == 0)
						effects.eq[4].level = atof(levelString.data);
					if (strcmp(frequencyString.data, "1kHz") == 0)
						effects.eq[5].level = atof(levelString.data);
					if (strcmp(frequencyString.data, "2kHz") == 0)
						effects.eq[6].level = atof(levelString.data);
					if (strcmp(frequencyString.data, "4kHz") == 0)
						effects.eq[7].level = atof(levelString.data);
					if (strcmp(frequencyString.data, "8kHz") == 0)
						effects.eq[8].level = atof(levelString.data);
					if (strcmp(frequencyString.data, "16kHz") == 0)
						effects.eq[9].level = atof(levelString.data);
				}

				lb_freeString(&frequencyString);
				lb_freeString(&levelString);
			}
			else if (strcmp(header.data, "crossfading") == 0)
			{
				effects.crossfading = atoi(value.data);
				if (strcmp(value.data, "none") == 0)
					effects.crossfading = 0;
			}
			else if (strcmp(header.data, "pitch blend") == 0)
			{
				effects.pitchBlend = atof(value.data);
				if (strcmp(value.data, "none") == 0)
					effects.pitchBlend = 0;
			}
			lb_clearString(&header);
			lb_clearString(&value);
			parseState = previousParseState;
			break;
		case '}':
			composition->timeLength = currentTime_s;
			currentTime_s = 0.0;
			currentNote = 0;
			barCount = 0;
			currentTrack++;
			parseState = LB_PARSE_STATE_READING_NOTHING;
			break;
		default:
			if (parseState == LB_PARSE_STATE_READING_HEADER)
			{
				lb_appendString(&header, script[readPosition]);
			}
			else if (parseState == LB_PARSE_STATE_READING_VALUE)
			{
				lb_appendString(&value, script[readPosition]);
			}
			else if (parseState == LB_PARSE_STATE_READING_TRACK_SCOPE)
			{
				if ((script[readPosition] >= 'A' && script[readPosition] <= 'G') ||
					script[readPosition] == 'R')
				{
					parseState = LB_PARSE_STATE_READING_NOTE_FREQUENCY;
					noteToPlay = script[readPosition];
					tuneByKeySignature(composition->keySignature, &noteToPlay);
					assignFrequencyFromNoteChar(&note.key, octave, noteToPlay);
				}
				else if (script[readPosition] == '-')
				{
					if (parseState != LB_PARSE_STATE_READING_VALUE)
						octave--;
				}
				else if (script[readPosition] == '+')
				{
					if (parseState != LB_PARSE_STATE_READING_VALUE)
						octave++;
				}
			}
			else if ((parseState == LB_PARSE_STATE_READING_NOTE_FREQUENCY || parseState == LB_PARSE_STATE_READING_NOTE_ACCIDENTAL) &&
				(script[readPosition] >= '1' && script[readPosition] <= '9'))
			{
				hasFractionalDuration = false;
				hasDottedDuration = false;
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
				hasDottedDuration = true;

				if (hasFractionalDuration)
					duration = 1.0 / atoi(durationString.data);
				else
					duration = atoi(durationString.data);

				duration += duration / 2.0;
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
				tuneByAccidental(&note.key, octave, script[readPosition], noteToPlay);
			}
		}
		readPosition++;
	} while (script[readPosition] != NULL);

	lb_freeString(&header);
	lb_freeString(&value);
	lb_freeString(&durationString);
}

void tuneByKeySignature(uint8_t keySignature, char* noteChar)
{

	if (keySignature == LB_KEY_SIGNATURE_C_MAJOR || keySignature == LB_KEY_SIGNATURE_A_MINOR)
	{
		;
	}
	else if (keySignature == LB_KEY_SIGNATURE_G_MAJOR || keySignature == LB_KEY_SIGNATURE_E_MINOR)
	{
		switch (*noteChar)
		{
			case 'F': *noteChar = 'f'; break;
		}
	}
	else if (keySignature == LB_KEY_SIGNATURE_D_MAJOR || keySignature == LB_KEY_SIGNATURE_B_MINOR)
	{
		switch (*noteChar)
		{
			case 'F': *noteChar = 'f'; break;
			case 'C': *noteChar = 'c'; break;
		}
	}
	else if (keySignature == LB_KEY_SIGNATURE_A_MAJOR || keySignature == LB_KEY_SIGNATURE_Fs_MINOR)
	{
		switch (*noteChar)
		{
			case 'F': *noteChar = 'f'; break;
			case 'C': *noteChar = 'c'; break;
			case 'G': *noteChar = 'g'; break;
		}
	}
	else if (keySignature == LB_KEY_SIGNATURE_E_MAJOR || keySignature == LB_KEY_SIGNATURE_Cs_MINOR)
	{
		switch (*noteChar)
		{
			case 'F': *noteChar = 'f'; break;
			case 'C': *noteChar = 'c'; break;
			case 'G': *noteChar = 'g'; break;
			case 'D': *noteChar = 'e'; break;
		}
	}
	else if (keySignature == LB_KEY_SIGNATURE_B_MAJOR || keySignature == LB_KEY_SIGNATURE_Cb_MAJOR ||
		keySignature == LB_KEY_SIGNATURE_Gs_MINOR)
	{
		switch (*noteChar)
		{
			case 'F': *noteChar = 'f'; break;
			case 'C': *noteChar = 'c'; break;
			case 'G': *noteChar = 'g'; break;
			case 'D': *noteChar = 'e'; break;
			case 'A': *noteChar = 'b'; break;
		}
	}
	else if (keySignature == LB_KEY_SIGNATURE_Fs_MAJOR || keySignature == LB_KEY_SIGNATURE_Ds_MINOR)
	{
		switch (*noteChar)
		{
			case 'F': *noteChar = 'f'; break;
			case 'C': *noteChar = 'c'; break;
			case 'G': *noteChar = 'g'; break;
			case 'D': *noteChar = 'e'; break;
			case 'A': *noteChar = 'b'; break;
			case 'E': *noteChar = 'F'; break;
		}
	}
	else if (keySignature == LB_KEY_SIGNATURE_Gb_MAJOR || keySignature == LB_KEY_SIGNATURE_Eb_MINOR)
	{
		switch (*noteChar)
		{
			case 'B': *noteChar = 'b'; break;
			case 'E': *noteChar = 'e'; break;
			case 'A': *noteChar = 'g'; break;
			case 'D': *noteChar = 'c'; break;
			case 'G': *noteChar = 'f'; break;
			case 'C': *noteChar = 'B'; break;
		}
	}
	else if (keySignature == LB_KEY_SIGNATURE_Db_MAJOR || keySignature == LB_KEY_SIGNATURE_Cs_MAJOR ||
		keySignature == LB_KEY_SIGNATURE_Bb_MINOR)
	{
		switch (*noteChar)
		{
			case 'B': *noteChar = 'b'; break;
			case 'E': *noteChar = 'e'; break;
			case 'A': *noteChar = 'g'; break;
			case 'D': *noteChar = 'c'; break;
			case 'G': *noteChar = 'f'; break;
		}
	}
	else if (keySignature == LB_KEY_SIGNATURE_Ab_MAJOR || keySignature == LB_KEY_SIGNATURE_F_MINOR)
	{
		switch (*noteChar)
		{
			case 'B': *noteChar = 'b'; break;
			case 'E': *noteChar = 'e'; break;
			case 'A': *noteChar = 'g'; break;
			case 'D': *noteChar = 'c'; break;
		}
	}
	else if (keySignature == LB_KEY_SIGNATURE_Eb_MAJOR || keySignature == LB_KEY_SIGNATURE_C_MINOR)
	{
		switch (*noteChar)
		{
			case 'B': *noteChar = 'b'; break;
			case 'E': *noteChar = 'e'; break;
			case 'A': *noteChar = 'g'; break;
		}
	}
	else if (keySignature == LB_KEY_SIGNATURE_Bb_MAJOR || keySignature == LB_KEY_SIGNATURE_G_MINOR)
	{
		switch (*noteChar)
		{
			case 'B': *noteChar = 'b'; break;
			case 'E': *noteChar = 'e'; break;
		}
	}
	else if (keySignature == LB_KEY_SIGNATURE_F_MAJOR || keySignature == LB_KEY_SIGNATURE_D_MINOR)
	{
		switch (*noteChar)
		{
			case 'B': *noteChar = 'b'; break;
		}
	}
}

void assignFrequencyFromNoteChar(float* frequency, uint8_t octave, char noteChar)
{
	switch (noteChar)
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

void tuneByAccidental(float* frequency, uint8_t octave, char scriptChar, char noteChar)
{
	switch (scriptChar)
	{
	case '#':
		switch (noteChar)
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
		switch (noteChar)
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
		switch (noteChar)
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