#pragma once

#include "../Utilities/Aliases.hpp"
#include "../Utilities/Collections/Set.hpp"
#include "../Utilities/Collections/Queue.hpp"
#include "../Utilities/Constants.hpp"
#include "StreamedAudio.hpp"

namespace Lilliputian
{
	class StreamedAudioSource2D
	{
	private:
		Set<AssetID> audioList;
		Queue<AssetID> audioPlayQueue;
	public:
		EntityID entityID = NO_ENTITY;

		void addStreamedAudio(AssetID audio);
		void queueAudioToPlay(AssetID audio, uint8_t count);
		Queue<AssetID> getAudioPlayQueue();
		void clearAudioPlayQueue();
	};
}