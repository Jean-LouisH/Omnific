#pragma once

#include "utilities/aliases.hpp"
#include "utilities/collections/set.hpp"
#include "utilities/collections/queue.hpp"
#include "utilities/constants.hpp"
#include "utilities/string.hpp"
#include "utilities/collections/vector.hpp"
#include "utilities/collections/map.hpp"
#include "game/scene/assets/audio_stream.hpp"
#include "component.hpp"

namespace Lilliputian
{
	class AudioStreamSource2D : public Component
	{
	public:
		void addAudioStream(AudioStream audioStream);
		void queueAudioToPlay(String audioStreamName, uint8_t count);
		void clearAudioStreams();
		Queue<AudioStream> popEntireAudioPlayQueue();
		void clearAudioPlayQueue();
		void play(String audioStreamName);
		void play();
		void pause();
		void stop();
		Vector<String> getAudioStreamNames();
		AudioStream getAudioStreamByName(String audioStreamName);
	private:
//		Map<String, AudioStream> audioStreams;
		Queue<AudioStream> audioPlayQueue;
		bool isPlaying = false;
	};
}