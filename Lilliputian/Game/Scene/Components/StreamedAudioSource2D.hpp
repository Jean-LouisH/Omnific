#pragma once

#include "../../../Utilities/Aliases.hpp"
#include "../../../Utilities/Collections/Set.hpp"
#include "../../../Utilities/Collections/Queue.hpp"
#include "../../../Utilities/Constants.hpp"
#include "../Assets/StreamedAudio.hpp"
#include "Component.hpp"

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