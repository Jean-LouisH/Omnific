#include "include/Mixer.h"
#include "include/Constants.h"

void interleaveWaveformToStream(Sint16 outputStream[], lb_Playback* playback)
{
	lb_Waveforms* waveforms = &playback->currentWaveforms;
	lb_Note* metaData = waveforms->noteMetaData;

	for (int i = 0; i < waveforms->count; i++)
	{
		if (metaData[i].panning == LB_PANNING_FRONT_LEFT)
			for (int j = 0; j < SAMPLE_SIZE; j++)
				outputStream[0 + (6 * j)] += playback->outputVolume * waveforms->streams[i][j] * CHANNELS;
		else if (metaData[i].panning == LB_PANNING_FRONT_RIGHT)
			for (int j = 0; j < SAMPLE_SIZE; j++)
				outputStream[1 + (6 * j)] += playback->outputVolume * waveforms->streams[i][j] * CHANNELS;
		else if (metaData[i].panning == LB_PANNING_CENTRE)
			for (int j = 0; j < SAMPLE_SIZE; j++)
				outputStream[2 + (6 * j)] += playback->outputVolume * waveforms->streams[i][j] * CHANNELS;
		else if (metaData[i].panning == LB_PANNING_CENTRE && metaData->key < 125)
			for (int j = 0; j < SAMPLE_SIZE; j++)
				outputStream[3 + (6 * j)] += playback->outputVolume * waveforms->streams[i][j] * CHANNELS;
		else if (metaData[i].panning == LB_PANNING_REAR_LEFT)
			for (int j = 0; j < SAMPLE_SIZE; j++)
				outputStream[4 + (6 * j)] += playback->outputVolume * waveforms->streams[i][j] * CHANNELS;
		else if (metaData[i].panning == LB_PANNING_REAR_RIGHT)
			for (int j = 0; j < SAMPLE_SIZE; j++)
				outputStream[5 + (6 * j)] += playback->outputVolume * waveforms->streams[i][j] * CHANNELS;
	}
}