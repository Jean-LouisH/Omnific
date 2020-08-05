#include "AudioSource2D.hpp"

void Lilliputian::AudioSource2D::addAudio(AssetID audio)
{
	this->audioList.emplace(audio);
}

void Lilliputian::AudioSource2D::queueAudioToPlay(AssetID audio, uint8_t count)
{
	if (this->audioList.count(audio) > 0 && count > 0)
	{
		for (int i = 0; i < count; i++)
			this->audioPlayQueue.emplace(audio);
	}
}

Lilliputian::Queue<Lilliputian::AssetID> Lilliputian::AudioSource2D::getAudioPlayQueue()
{
	return this->audioPlayQueue;
}

void Lilliputian::AudioSource2D::clearAudioPlayQueue()
{
	for (int i = 0; i < this->audioPlayQueue.size(); i++)
		this->audioPlayQueue.pop();
}