#include "Mixer.h"
#include "Constants.h"

void interleaveNoteWavesToStream(Sint16 outputStream[], lb_NoteWaves* noteWaves)
{
	for (int i = 0; i < noteWaves->count; i++)
	{
		if (noteWaves->metaData[i].panning == FRONT_LEFT)
			for (int j = 0; j < SAMPLE_SIZE; j++)
				outputStream[0 + (6 * j)] += noteWaves->streams[i][j] * CHANNELS;
		else if (noteWaves->metaData[i].panning == FRONT_RIGHT)
			for (int j = 0; j < SAMPLE_SIZE; j++)
				outputStream[1 + (6 * j)] += noteWaves->streams[i][j] * CHANNELS;
		else if (noteWaves->metaData[i].panning == CENTRE)
			for (int j = 0; j < SAMPLE_SIZE; j++)
				outputStream[2 + (6 * j)] += noteWaves->streams[i][j] * CHANNELS;
		else if (noteWaves->metaData[i].panning == CENTRE && noteWaves->metaData->frequency_Hz < 125)
			for (int j = 0; j < SAMPLE_SIZE; j++)
				outputStream[3 + (6 * j)] += noteWaves->streams[i][j] * CHANNELS;
		else if (noteWaves->metaData[i].panning == REAR_LEFT)
			for (int j = 0; j < SAMPLE_SIZE; j++)
				outputStream[4 + (6 * j)] += noteWaves->streams[i][j] * CHANNELS;
		else if (noteWaves->metaData[i].panning == REAR_RIGHT)
			for (int j = 0; j < SAMPLE_SIZE; j++)
				outputStream[5 + (6 * j)] += noteWaves->streams[i][j] * CHANNELS;
	}
}