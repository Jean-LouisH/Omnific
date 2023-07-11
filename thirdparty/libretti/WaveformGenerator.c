#include "include/WaveformGenerator.h"
#include <stdbool.h>
#include <math.h>
#include "Constants.h"

void generateWaveform(lb_Waveforms* waveforms, lb_Note currentNotes[])
{
	static double timesteps[MAX_TRACKS];

	for (int i = 0; i < waveforms->count; i++)
	{
		waveforms->noteMetaData[i] = currentNotes[i];

		for (int n = 0; n < SAMPLE_SIZE; n++)
		{
			double frequency = waveforms->noteMetaData[i].key;
			double time = (timesteps[i] / SAMPLE_FREQUENCY);
			double period = (1.0 / frequency);
			double amplitude = waveforms->noteMetaData[i].dynamic;

			switch (waveforms->noteMetaData[i].timbre)
			{
				case LB_TIMBRE_SQUARE_WAVE:
					waveforms->streams[i][n] = amplitude * pow((-1), (int)(frequency * time));
					break;
				case LB_TIMBRE_SINE_WAVE:
					waveforms->streams[i][n] = amplitude * sin(2.0 * M_PI * frequency * time);
					break;
				case LB_TIMBRE_TRIANGLE_WAVE:
					waveforms->streams[i][n] = (2.0 * amplitude / M_PI) * asin(sin((2.0 * M_PI / period) * time));
					break;
				case LB_TIMBRE_SAWTOOTH_WAVE:
					waveforms->streams[i][n] = 2 * amplitude * (time / period - (int)(0.5 + time / period));
					break;
				case LB_TIMBRE_PULSE_10:
					waveforms->streams[i][n] = amplitude * 
						(2 * amplitude * (time / period - (int)(0.5 + time / period)) > (amplitude * (1.0 - 0.10)));
					break;
				case LB_TIMBRE_PULSE_25:
					waveforms->streams[i][n] = amplitude *
						(2 * amplitude * (time / period - (int)(0.5 + time / period)) > (amplitude * (1.0 - 0.25)));
					break;
				case LB_TIMBRE_NOISE:
					waveforms->streams[i][n] = (double)((bool)(frequency) * 
						(rand() % (int)(2 * amplitude) + (int)(-amplitude)));
					break;
				case LB_TIMBRE_SAMPLE:
					waveforms->streams[i][n] = amplitude * (waveforms->noteMetaData[i].sample[(int)(frequency * time * SAMPLE_SIZE) % SAMPLE_SIZE]) / (pow(2.0, (8.0 * sizeof(int16_t) - 1)) - 1) ;
					break;
			}

			if (frequency <= 0)
				waveforms->streams[i][n] = 0.0;

			timesteps[i]++;
		}
	}
}