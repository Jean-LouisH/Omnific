#include "Compiler.h"
#include "File.h"
#include "ScriptParseStates.h"
#include "ScriptValidator.h"
#include "Strings.h"
#include "Timing.h"
#include "Notes.h"
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

void compileAudioFromScript(lb_Audio* audio, const char* script)
{
	if (validateScript(script) == ALL_OK)
	{
		allocateMemory(audio, script);
		buildAudioData(audio, script);
	}
	else
	{
		printf("\nCompilation Failed.");
	}
}

void allocateMemory(lb_Audio* audio, const char* script)
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
				audio->tracks[currentTrack].noteEvents = malloc(sizeof(lb_NoteEvent) * noteCount);
			else
				audio->tracks[currentTrack].noteEvents = calloc(1, sizeof(lb_NoteEvent));
			audio->tracks[currentTrack].noteCount = noteCount;
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

	audio->tempoEvents = malloc(sizeof(lb_TempoEvent) * tempoEventCount);
	audio->tempoEventCount = tempoEventCount;
	audio->lyricsEvents = malloc(sizeof(lb_LyricsEvent) * lyricsEventCount);
	audio->lyricsEventCount = lyricsEventCount;
	audio->trackCount = currentTrack + 1;
	lb_freeString(&header);
}

void buildAudioData(lb_Audio* audio, const char* script)
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
	uint8_t tempo = 0;
	uint16_t dynamic = 0;
	int8_t panning = 0;
	uint8_t timbre = 0;
	uint16_t cue = 0;
	double duration = 0.0;
	lb_Binary_s16 sample = { NULL, 0 };
	bool tupletIsOpened = false;
	bool slurIsOpened = false;
	bool isReadingCrescendo = false;
	bool isReadingDiminuendo = false;
	bool hasFractionalDuration = false;

	lb_Note note;
	lb_Effects effects;
	effects.echo.decay_pct = 0.0;
	effects.echo.delay_ms = 0.0;
	effects.reverb.damping_pct = 0.0;
	effects.reverb.preDelay_ms = 0.0;
	effects.reverb.roomSize_pct = 0.0;
	effects.vibrato.extent_cents = 0.0;
	effects.vibrato.rate_per_s = 0.0;
	effects.crossfading_ms = 0.0;
	effects.pitchBlend_pct = 0.0;

	for (int i = 0; i < 10; i++)
		effects.eq[i].level_dB = 0;

	char noteToPlay;

	uint8_t parseState = READING_NOTHING;
	uint8_t previousParseState = 0;
	lb_String header = lb_newString("");
	lb_String value = lb_newString("");

	lb_String durationString = lb_newString("");

	do
	{
		switch (script[readPosition])
		{
		case '{':
			parseState = READING_TRACK_SCOPE;
			break;
		case '[':
			previousParseState = parseState;
			parseState = READING_HEADER;
			break;
		case '_':
			tupletIsOpened = !tupletIsOpened;
			break;
		case '~':
			slurIsOpened = !slurIsOpened;
			break;
		case '<':
			isReadingCrescendo = !isReadingCrescendo;
			break;
		case '>':
			if (parseState == READING_NOTE_DURATION)
				parseState = READING_DYNAMIC_ACCENT;
			else
				isReadingDiminuendo = !isReadingDiminuendo;
			break;
		case '*':
			if (parseState == READING_NOTE_DURATION)
				parseState = READING_STACCATO;
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

			if (parseState == READING_NOTE_DURATION ||
				parseState == READING_DYNAMIC_ACCENT ||
				parseState == READING_STACCATO)
			{
				if (parseState == READING_DYNAMIC_ACCENT)
					note.amplitude = dynamic * 2;
				else
					note.amplitude = dynamic;

				if (parseState == READING_STACCATO)
					note.articulation = STACCATO;
				else if (slurIsOpened)
					note.articulation = SLUR;
				else
					note.articulation = NORMAL;
				note.cue = cue;
				note.panning = panning;
				note.sample = sample;
				note.timbre = timbre;
				note.effects = effects;

				audio->tracks[currentTrack].noteEvents[currentNote].note = note;
				audio->tracks[currentTrack].noteEvents[currentNote].startTime_s = currentTime_s;

				if (hasFractionalDuration)
					duration = 1.0 / atoi(durationString.data);
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
				parseState = READING_TRACK_SCOPE;
				currentNote++;
			}

			if (parseState == IGNORING_FIRST_SPACE_IN_VALUE)
			{
				parseState = READING_VALUE;
			}
			else if (parseState == READING_HEADER)
			{
				lb_appendString(&header, script[readPosition]);
			}
			else if (parseState == READING_VALUE)
			{
				lb_appendString(&value, script[readPosition]);
			}
			break;
		case ':':
			parseState = IGNORING_FIRST_SPACE_IN_VALUE;
			break;
		case ']':
			if (strcmp(header.data, "name") == 0)
			{
				int valueReadPosition = 0;
				while (value.data[valueReadPosition] != 0)
				{
					audio->name[valueReadPosition] = value.data[valueReadPosition];
					valueReadPosition++;
				}
			}
			else if (strcmp(header.data, "artist") == 0)
			{
				int valueReadPosition = 0;
				while (value.data[valueReadPosition] != 0)
				{
					audio->artist[valueReadPosition] = value.data[valueReadPosition];
					valueReadPosition++;
				}
			}
			else if (strcmp(header.data, "key sig") == 0)
			{
				if (strcmp(value.data, "C major") == 0)
					audio->keySignature = C_MAJOR;
				else if (strcmp(value.data, "G major") == 0)
					audio->keySignature = G_MAJOR;
				else if (strcmp(value.data, "D major") == 0)
					audio->keySignature = D_MAJOR;
				else if (strcmp(value.data, "A major") == 0)
					audio->keySignature = A_MAJOR;
				else if (strcmp(value.data, "E major") == 0)
					audio->keySignature = E_MAJOR;
				else if (strcmp(value.data, "B major") == 0)
					audio->keySignature = B_MAJOR;
				else if (strcmp(value.data, "Fs major") == 0)
					audio->keySignature = Fs_MAJOR;
				else if (strcmp(value.data, "Gb major") == 0)
					audio->keySignature = Gb_MAJOR;
				else if (strcmp(value.data, "Db major") == 0)
					audio->keySignature = Db_MAJOR;
				else if (strcmp(value.data, "Ab major") == 0)
					audio->keySignature = Ab_MAJOR;
				else if (strcmp(value.data, "Eb major") == 0)
					audio->keySignature = Eb_MAJOR;
				else if (strcmp(value.data, "Bb major") == 0)
					audio->keySignature = Bb_MAJOR;
				else if (strcmp(value.data, "F major") == 0)
					audio->keySignature = F_MAJOR;
				else if (strcmp(value.data, "A minor") == 0)
					audio->keySignature = A_MINOR;
				else if (strcmp(value.data, "E minor") == 0)
					audio->keySignature = E_MINOR;
				else if (strcmp(value.data, "B minor") == 0)
					audio->keySignature = B_MINOR;
				else if (strcmp(value.data, "Fs minor") == 0)
					audio->keySignature = Fs_MINOR;
				else if (strcmp(value.data, "Cs minor") == 0)
					audio->keySignature = Cs_MINOR;
				else if (strcmp(value.data, "Gs minor") == 0)
					audio->keySignature = Gs_MINOR;
				else if (strcmp(value.data, "Ds minor") == 0)
					audio->keySignature = Ds_MINOR;
				else if (strcmp(value.data, "Eb minor") == 0)
					audio->keySignature = Eb_MINOR;
				else if (strcmp(value.data, "Bb minor") == 0)
					audio->keySignature = Bb_MINOR;
				else if (strcmp(value.data, "F minor") == 0)
					audio->keySignature = F_MINOR;
				else if (strcmp(value.data, "C minor") == 0)
					audio->keySignature = C_MINOR;
				else if (strcmp(value.data, "G minor") == 0)
					audio->keySignature = G_MINOR;
				else if (strcmp(value.data, "D minor") == 0)
					audio->keySignature = D_MINOR;
			}
			else if (strcmp(header.data, "time sig") == 0)
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

				audio->timeSignature[0] = timeSigUpper;
				audio->timeSignature[1] = timeSigLower;

				lb_freeString(&upper);
				lb_freeString(&lower);
			}
			else if (strcmp(header.data, "tempo") == 0)
			{
				tempo = atoi(value.data);
				if (tempo == 0)
				{
					if (strcmp(value.data, "largo") == 0)
						tempo = LARGO;
					else if (strcmp(value.data, "adagio") == 0)
						tempo = ADAGIO;
					else if (strcmp(value.data, "adante") == 0)
						tempo = ADANTE;
					else if (strcmp(value.data, "moderato") == 0)
						tempo = MODERATO;
					else if (strcmp(value.data, "allegro") == 0)
						tempo = ALLEGRO;
					else if (strcmp(value.data, "presto") == 0)
						tempo = PRESTO;
				}
				audio->tempoEvents[currentTempoEvent].tempo_bpm = tempo;
				audio->tempoEvents[currentTempoEvent].startTime_s = currentTime_s;
				currentTempoEvent++;
			}
			else if (strcmp(header.data, "dynamic") == 0)
			{
				if (strcmp(value.data, "ppp") == 0)
					dynamic = PPP;
				else if (strcmp(value.data, "pp") == 0)
					dynamic = PP;
				else if (strcmp(value.data, "p") == 0)
					dynamic = P;
				else if (strcmp(value.data, "mp") == 0)
					dynamic = MP;
				else if (strcmp(value.data, "mf") == 0)
					dynamic = MF;
				else if (strcmp(value.data, "f") == 0)
					dynamic = F;
				else if (strcmp(value.data, "ff") == 0)
					dynamic = FF;
				else if (strcmp(value.data, "fff") == 0)
					dynamic = FFF;
			}
			else if (strcmp(header.data, "reverb") == 0)
			{
				float reverbValue = atof(value.data);
				if (reverbValue == 0)
				{
					if (strcmp(value.data, "standard") == 0)
					{
						effects.reverb.damping_pct = 50;
						effects.reverb.preDelay_ms = 10;
						effects.reverb.roomSize_pct = 50;
					}
					else if (strcmp(value.data, "none") == 0)
					{
						effects.reverb.damping_pct = 0;
						effects.reverb.preDelay_ms = 0;
						effects.reverb.roomSize_pct = 0;
					}
				}
			}
			else if (strcmp(header.data, "vibrato") == 0)
			{
				if (strcmp(value.data, "standard") == 0)
				{
					effects.vibrato.extent_cents = 100;
					effects.vibrato.rate_per_s = 8;
				}
				else if (strcmp(value.data, "none") == 0)
				{
					effects.vibrato.extent_cents = 0;
					effects.vibrato.rate_per_s = 0;
				}
			}
			else if (strcmp(header.data, "panning") == 0)
			{
				float panningValue = atof(value.data);
				if (panningValue == 0)
				{
					if (strcmp(value.data, "far left") == 0)
						panning = REAR_LEFT;
					else if (strcmp(value.data, "left") == 0)
						panning = FRONT_LEFT;
					else if (strcmp(value.data, "mono") == 0)
						panning = CENTRE;
					else if (strcmp(value.data, "right") == 0)
						panning = FRONT_RIGHT;
					else if (strcmp(value.data, "far right") == 0)
						panning = REAR_RIGHT;
				}
				else
				{
					panning = panningValue * 127;
				}
			}
			else if (strcmp(header.data, "timbre") == 0)
			{
				if (strcmp(value.data, "square wave") == 0)
					timbre = SQUARE_WAVE;
				else if (strcmp(value.data, "sine wave") == 0)
					timbre = SINE_WAVE;
				else if (strcmp(value.data, "triangle wave") == 0)
					timbre = TRIANGLE_WAVE;
				else if (strcmp(value.data, "sawtooth wave") == 0)
					timbre = SAWTOOTH_WAVE;
				else if (strcmp(value.data, "pulse 10") == 0)
					timbre = PULSE_10;
				else if (strcmp(value.data, "pulse 25") == 0)
					timbre = PULSE_25;
				else if (strcmp(value.data, "noise") == 0)
					timbre = NOISE;
				else if (strcmp(value.data, "metallic") == 0)
					timbre = METALLIC;
				else
				{
#ifdef _DEBUG
					lb_String filename = lb_newString("../Libretti/Samples/");
#else
					lb_String filename = newString("Samples/");
#endif
					lb_String extension = lb_newString(".pcm");
					timbre = SAMPLE;
					strcat(filename.data, value.data);
					strcat(filename.data, extension.data);
					sample = loadBinaryS16FromFile(filename.data);
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
				audio->loopTimestamp_s = currentTime_s;
				audio->loopCount = atoi(value.data);
				if (strcmp(value.data, "infinity") == 0 || 
					audio->loopCount > pow(2, (sizeof audio->loopCount) * 8) - 1)
					audio->loopCount = pow(2, (sizeof audio->loopCount) * 8) - 1;
			}
			else if (strcmp(header.data, "cue") == 0)
			{
				cue = atoi(value.data);
			}
			else if (strcmp(header.data, "echo") == 0)
			{
				if (strcmp(value.data, "standard") == 0)
				{
					effects.echo.decay_pct = 50;
					effects.echo.delay_ms = 1000;
				}
				else if (strcmp(value.data, "none") == 0)
				{
					effects.echo.decay_pct = 0;
					effects.echo.delay_ms = 0;
				}
			}
			else if (strcmp(header.data, "lyric") == 0)
			{
				//audio->lyricsEvents[currentLyricsEvent].lyrics = malloc(sizeof(char) * value.capacity);
				strcpy(audio->lyricsEvents[currentLyricsEvent].lyrics, value.data);
				audio->lyricsEvents[currentLyricsEvent].startTime_s = currentTime_s;
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
						effects.eq[i].level_dB = 0;
				}
				else
				{
					if (strcmp(frequencyString.data, "31Hz") == 0)
						effects.eq[0].level_dB = atof(levelString.data);
					if (strcmp(frequencyString.data, "62Hz") == 0)
						effects.eq[1].level_dB = atof(levelString.data);
					if (strcmp(frequencyString.data, "125Hz") == 0)
						effects.eq[2].level_dB = atof(levelString.data);
					if (strcmp(frequencyString.data, "250Hz") == 0)
						effects.eq[3].level_dB = atof(levelString.data);
					if (strcmp(frequencyString.data, "500Hz") == 0)
						effects.eq[4].level_dB = atof(levelString.data);
					if (strcmp(frequencyString.data, "1kHz") == 0)
						effects.eq[5].level_dB = atof(levelString.data);
					if (strcmp(frequencyString.data, "2kHz") == 0)
						effects.eq[6].level_dB = atof(levelString.data);
					if (strcmp(frequencyString.data, "4kHz") == 0)
						effects.eq[7].level_dB = atof(levelString.data);
					if (strcmp(frequencyString.data, "8kHz") == 0)
						effects.eq[8].level_dB = atof(levelString.data);
					if (strcmp(frequencyString.data, "16kHz") == 0)
						effects.eq[9].level_dB = atof(levelString.data);
				}

				lb_freeString(&frequencyString);
				lb_freeString(&levelString);
			}
			else if (strcmp(header.data, "crossfading") == 0)
			{
				effects.crossfading_ms = atoi(value.data);
				if (strcmp(value.data, "none") == 0)
					effects.crossfading_ms = 0;
			}
			else if (strcmp(header.data, "pitch blend") == 0)
			{
				effects.pitchBlend_pct = atof(value.data);
				if (strcmp(value.data, "none") == 0)
					effects.pitchBlend_pct = 0;
			}
			lb_clearString(&header);
			lb_clearString(&value);
			parseState = previousParseState;
			break;
		case '}':
			audio->timeLength_s = currentTime_s;
			currentTime_s = 0.0;
			currentNote = 0;
			barCount = 0;
			currentTrack++;
			parseState = READING_NOTHING;
			break;
		default:
			if (parseState == READING_HEADER)
			{
				lb_appendString(&header, script[readPosition]);
			}
			else if (parseState == READING_VALUE)
			{
				lb_appendString(&value, script[readPosition]);
			}
			else if (parseState == READING_TRACK_SCOPE)
			{
				if ((script[readPosition] >= 'A' && script[readPosition] <= 'G') ||
					script[readPosition] == 'R')
				{
					parseState = READING_NOTE_FREQUENCY;
					noteToPlay = script[readPosition];
					tuneByKeySignature(audio->keySignature, &noteToPlay);
					assignFrequencyFromNoteChar(&note.frequency_Hz, octave, noteToPlay);
				}
				else if (script[readPosition] == '-')
				{
					if (parseState != READING_VALUE)
						octave--;
				}
				else if (script[readPosition] == '+')
				{
					if (parseState != READING_VALUE)
						octave++;
				}
			}
			else if ((parseState == READING_NOTE_FREQUENCY || parseState == READING_NOTE_ACCIDENTAL) &&
				(script[readPosition] >= '1' && script[readPosition] <= '9'))
			{
				hasFractionalDuration = false;
				parseState = READING_NOTE_DURATION;
				lb_appendString(&durationString, script[readPosition]);
			}
			else if ((parseState == READING_NOTE_FREQUENCY || parseState == READING_NOTE_ACCIDENTAL) &&
				(script[readPosition] == '/'))
			{
				hasFractionalDuration = true;
				parseState = READING_NOTE_DURATION;
			}
			else if (parseState == READING_NOTE_DURATION && script[readPosition] == '.')
			{
				if (hasFractionalDuration)
					duration = 1.0 / atoi(durationString.data);
				else
					duration = atoi(durationString.data);

				duration += duration / 2.0;

				lb_clearString(&durationString);

				if (tupletIsOpened)
					duration = (duration * 2.0) / 3.0;

				double secondsPerBeat = 60.0 / (double)tempo;
				double beatLength = 4.0 / (double)timeSigLower;
				secondsPerBeat /= beatLength;
				duration *= secondsPerBeat;
				currentTime_s += duration;
				parseState = READING_TRACK_SCOPE;
				currentNote++;
			}
			else if (parseState == READING_NOTE_DURATION && 
				(script[readPosition] >= '0' && script[readPosition] <= '9'))
			{
				lb_appendString(&durationString, script[readPosition]);
			}
			else if (parseState == READING_NOTE_FREQUENCY &&
				(script[readPosition] == '#' || script[readPosition] == 'b' ||
					script[readPosition] == 'n'))
			{
				parseState = READING_NOTE_ACCIDENTAL;
				tuneByAccidental(&note.frequency_Hz, octave, script[readPosition], noteToPlay);
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
	if (keySignature != C_MAJOR && keySignature != A_MINOR)
	{
		switch (*noteChar)
		{
			case 'F': 
				*noteChar = 'f'; 
				break;
			case 'C':
				if (keySignature != G_MAJOR)
					*noteChar = 'c';
				break;
			case 'G':
				if (keySignature != G_MAJOR && keySignature != D_MAJOR)
					*noteChar = 'g';
				break;
			case 'D':
				if (keySignature != G_MAJOR && keySignature != D_MAJOR &&
					keySignature != A_MAJOR)
					*noteChar = 'e';
				break;
			case 'A':
				if (keySignature != G_MAJOR && keySignature != D_MAJOR &&
					keySignature != A_MAJOR && keySignature != E_MAJOR)
					*noteChar = 'b';
				break;
			case 'E':
				if (keySignature != G_MAJOR && keySignature != D_MAJOR &&
					keySignature != A_MAJOR && keySignature != E_MAJOR &&
					keySignature != B_MAJOR)
					*noteChar = 'F';
				break;
		}
	}
}

void assignFrequencyFromNoteChar(float* frequency, uint8_t octave, char noteChar)
{
	switch (noteChar)
	{
		case 'A': *frequency = A0 * pow(2, octave); break;
		case 'B': *frequency = B0 * pow(2, octave); break;
		case 'C': *frequency = C1 * pow(2, octave - 1); break;
		case 'D': *frequency = D1 * pow(2, octave - 1); break;
		case 'E': *frequency = E1 * pow(2, octave - 1); break;
		case 'F': *frequency = F1 * pow(2, octave - 1); break;
		case 'G': *frequency = G1 * pow(2, octave - 1); break;
			/*Flats and sharps char codes reserved for compiler.*/
		case 'b': *frequency = Bf0 * pow(2, octave); break;
		case 'c': *frequency = Cs1 * pow(2, octave - 1); break;
		case 'e': *frequency = Ef1 * pow(2, octave - 1); break;
		case 'f': *frequency = Fs1 * pow(2, octave - 1); break;
		case 'g': *frequency = Gs1 * pow(2, octave - 1); break;
			/*Rest value.*/
		case 'R': *frequency = R; break;
	}
}

void tuneByAccidental(float* frequency, uint8_t octave, char scriptChar, char noteChar)
{
	switch (scriptChar)
	{
	case '#':
		switch (noteChar)
		{
			case 'A': *frequency = Bf0 * pow(2, octave); break;
			case 'B': *frequency = C1 * pow(2, octave - 1); break;
			case 'C': *frequency = Cs1 * pow(2, octave - 1); break;
			case 'D': *frequency = Ef1 * pow(2, octave - 1); break;
			case 'E': *frequency = F1 * pow(2, octave - 1); break;
			case 'F': *frequency = Fs1 * pow(2, octave - 1); break;
			case 'G': *frequency = Gs1 * pow(2, octave - 1); break;
		}
		break;
	case 'b':
		switch (noteChar)
		{
			case 'A': *frequency = Gs1 * pow(2, octave - 1); break;
			case 'B': *frequency = Bf0 * pow(2, octave); break;
			case 'C': *frequency = B0 * pow(2, octave); break;
			case 'D': *frequency = Cs1 * pow(2, octave - 1); break;
			case 'E': *frequency = Ef1 * pow(2, octave - 1); break;
			case 'F': *frequency = E1 * pow(2, octave - 1); break;
			case 'G': *frequency = Fs1 * pow(2, octave - 1); break;
		}
		break;
	case 'n':
		switch (noteChar)
		{
			case 'b': *frequency = B0 * pow(2, octave); break;
			case 'c': *frequency = C1 * pow(2, octave - 1); break;
			case 'e': *frequency = E1 * pow(2, octave - 1); break;
			case 'f': *frequency = F1 * pow(2, octave - 1); break;
			case 'g': *frequency = G1 * pow(2, octave - 1); break;
		}
		break;
	}
}