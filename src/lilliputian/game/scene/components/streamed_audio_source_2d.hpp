#pragma once

#include "utilities/aliases.hpp"
#include "utilities/collections/set.hpp"
#include "utilities/collections/queue.hpp"
#include "utilities/constants.hpp"
#include "game/scene/assets/streamed_audio.hpp"
#include "component.hpp"

namespace Lilliputian
{
	class StreamedAudioSource2D : public Component
	{
	public:
		void addStreamedAudio(AssetID audio);
		void queueAudioToPlay(AssetID audio, uint8_t count);
		Queue<AssetID> getAudioPlayQueue();
		void clearAudioPlayQueue();
	private:
		Set<AssetID> audioList;
		Queue<AssetID> audioPlayQueue;
	};
}