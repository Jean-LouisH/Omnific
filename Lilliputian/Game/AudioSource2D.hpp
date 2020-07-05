#pragma once

#include "../Utilities/Aliases.hpp"
#include "../Utilities/Collections/Set.hpp"
#include "../Utilities/Collections/Queue.hpp"
#include "Audio.hpp"

namespace Lilliputian
{
	class AudioSource2D
	{
	private:
		EntityID entityID;
		Set<AssetID> audioList;
		Queue<AssetID> audioPlayQueue;
	public:
		AudioSource2D(EntityID entityID);
		void addAudio(AssetID audio);
		void queueAudioToPlay(AssetID audio, uint8_t count);
		Queue<AssetID> getAudioPlayQueue();
		void clearAudioPlayQueue();
		EntityID getEntityID();
	};
}