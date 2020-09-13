#include "WaveformGenerator.h"
#include <stdbool.h>
#include <math.h>

void generateNoteWaves(lb_NoteWaves* noteWaves, lb_Note currentNotes[])
{
	static double timesteps[MAX_TRACKS];

	for (int i = 0; i < noteWaves->count; i++)
	{
		noteWaves->metaData[i] = currentNotes[i];

		for (int n = 0; n < SAMPLE_SIZE; n++)
		{
			double frequency = noteWaves->metaData[i].frequency_Hz;
			double time = (timesteps[i] / SAMPLE_FREQUENCY);
			double period = (1.0 / noteWaves->metaData[i].frequency_Hz);
			double amplitude = noteWaves->metaData[i].amplitude;

			switch (noteWaves->metaData[i].timbre)
			{
				case SQUARE_WAVE:
					noteWaves->streams[i][n] = amplitude * pow((-1), (int)(frequency * time));
					break;
				case SINE_WAVE:
					noteWaves->streams[i][n] = amplitude * sin(2.0 * M_PI * frequency * time);
					break;
				case TRIANGLE_WAVE:
					noteWaves->streams[i][n] = (2.0 * amplitude / M_PI) * asin(sin((2.0 * M_PI / period) * time));
					break;
				case SAWTOOTH_WAVE:
					noteWaves->streams[i][n] = 2 * amplitude * (time / period - (int)(0.5 + time / period));
					break;
				case PULSE_10:
					noteWaves->streams[i][n] = amplitude * 
						(2 * amplitude * (time / period - (int)(0.5 + time / period)) > (amplitude * (1.0 - 0.10)));
					break;
				case PULSE_25:
					noteWaves->streams[i][n] = amplitude *
						(2 * amplitude * (time / period - (int)(0.5 + time / period)) > (amplitude * (1.0 - 0.25)));
					break;
				case NOISE:
					noteWaves->streams[i][n] = (double)((bool)(frequency) * 
						(rand() % (int)(2 * amplitude) + (int)(-amplitude)));
					break;
			}

			if (frequency <= 0)
				noteWaves->streams[i][n] = 0.0;

			timesteps[i]++;
		}
	}
}