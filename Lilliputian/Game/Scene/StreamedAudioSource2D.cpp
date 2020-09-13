#include "StreamedAudioSource2D.hpp"

void Lilliputian::StreamedAudioSource2D::addStreamedAudio(AssetID audio)
{
	this->audioList.emplace(audio);
}

void Lilliputian::StreamedAudioSource2D::queueAudioToPlay(AssetID audio, uint8_t count)
{
	if (this->audioList.count(audio) > 0 && count > 0)
	{
		for (int i = 0; i < count; i++)
			this->audioPlayQueue.emplace(audio);
	}
}

Lilliputian::Queue<Lilliputian::AssetID> Lilliputian::StreamedAudioSource2D::getAudioPlayQueue()
{
	return this->audioPlayQueue;
}

void Lilliputian::StreamedAudioSource2D::clearAudioPlayQueue()
{
	for (int i = 0; i < this->audioPlayQueue.size(); i++)
		this->audioPlayQueue.pop();
}