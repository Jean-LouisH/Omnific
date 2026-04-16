#include "mixer.h"
#include "constants.h"

void interleave_waveform_to_stream(int16_t* output_stream, lb_Playback* playback, uint8_t channel_count)
{
	lb_Waveforms* waveforms = &playback->current_waveforms;
	lb_Note* meta_data = waveforms->note_meta_data;

	if (channel_count == 1)
	{
		for (int i = 0; i < waveforms->count; i++)
			for (int j = 0; j < DEFAULT_STREAM_SAMPLE_SIZE; j++)
				output_stream[channel_count * j] += playback->output_volume * waveforms->streams[i][j] * channel_count;
	}
	else if (channel_count == 2)
	{
		for (int i = 0; i < waveforms->count; i++)
		{
			if (meta_data[i].panning == LB_PANNING_FRONT_LEFT)
				for (int j = 0; j < DEFAULT_STREAM_SAMPLE_SIZE; j++)
					output_stream[0 + (channel_count * j)] += playback->output_volume * waveforms->streams[i][j] * channel_count;
			else if (meta_data[i].panning == LB_PANNING_FRONT_RIGHT)
				for (int j = 0; j < DEFAULT_STREAM_SAMPLE_SIZE; j++)
					output_stream[1 + (channel_count * j)] += playback->output_volume * waveforms->streams[i][j] * channel_count;
			else if (meta_data[i].panning == LB_PANNING_CENTRE)
				for (int j = 0; j < DEFAULT_STREAM_SAMPLE_SIZE; j++)
				{
					output_stream[0 + (channel_count * j)] += playback->output_volume * waveforms->streams[i][j] * channel_count;
					output_stream[1 + (channel_count * j)] += playback->output_volume * waveforms->streams[i][j] * channel_count;
				}
		}
	}
	else if (channel_count > 2)
	{
		for (int i = 0; i < waveforms->count; i++)
		{
			if (meta_data[i].panning == LB_PANNING_FRONT_LEFT)
				for (int j = 0; j < DEFAULT_STREAM_SAMPLE_SIZE; j++)
					output_stream[0 + (channel_count * j)] += playback->output_volume * waveforms->streams[i][j] * channel_count;
			else if (meta_data[i].panning == LB_PANNING_FRONT_RIGHT)
				for (int j = 0; j < DEFAULT_STREAM_SAMPLE_SIZE; j++)
					output_stream[1 + (channel_count * j)] += playback->output_volume * waveforms->streams[i][j] * channel_count;
			else if (meta_data[i].panning == LB_PANNING_CENTRE)
				for (int j = 0; j < DEFAULT_STREAM_SAMPLE_SIZE; j++)
					output_stream[2 + (channel_count * j)] += playback->output_volume * waveforms->streams[i][j] * channel_count;
			else if (meta_data[i].panning == LB_PANNING_CENTRE && meta_data[i].key < 125 && channel_count > 3)
				for (int j = 0; j < DEFAULT_STREAM_SAMPLE_SIZE; j++)
					output_stream[3 + (channel_count * j)] += playback->output_volume * waveforms->streams[i][j] * channel_count;
			else if (meta_data[i].panning == LB_PANNING_REAR_LEFT && channel_count > 4)
				for (int j = 0; j < DEFAULT_STREAM_SAMPLE_SIZE; j++)
					output_stream[4 + (channel_count * j)] += playback->output_volume * waveforms->streams[i][j] * channel_count;
			else if (meta_data[i].panning == LB_PANNING_REAR_RIGHT && channel_count > 5)
				for (int j = 0; j < DEFAULT_STREAM_SAMPLE_SIZE; j++)
					output_stream[5 + (channel_count * j)] += playback->output_volume * waveforms->streams[i][j] * channel_count;
		}
	}
}