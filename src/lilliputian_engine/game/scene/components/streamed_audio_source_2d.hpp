#pragma once

#include "utilities/aliases.hpp"
#include "utilities/collections/set.hpp"
#include "utilities/collections/queue.hpp"
#include "utilities/constants.hpp"
#include "utilities/string.hpp"
#include "utilities/collections/vector.hpp"
#include "utilities/collections/map.hpp"
#include "game/scene/assets/streamed_audio.hpp"
#include "component.hpp"

namespace Lilliputian
{
	class StreamedAudioSource2D : public Component
	{
	public:
		void addStreamedAudio(StreamedAudio streamedAudio);
		void queueAudioToPlay(String streamedAudioName, uint8_t count);
		void clearStreamedAudios();
		Queue<StreamedAudio> popEntireAudioPlayQueue();
		void clearAudioPlayQueue();
		void play(String streamedAudioName);
		void play();
		void pause();
		void stop();
		Vector<String> getStreamedAudioNames();
		StreamedAudio getStreamedAudioByName(String streamedAudioName);
	private:
//		Map<String, StreamedAudio> streamedAudios;
		Queue<StreamedAudio> audioPlayQueue;
		bool isPlaying = false;
	};
}