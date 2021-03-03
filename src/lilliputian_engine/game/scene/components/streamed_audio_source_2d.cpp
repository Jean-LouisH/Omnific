#include "streamed_audio_source_2d.hpp"

void Lilliputian::StreamedAudioSource2D::addStreamedAudio(StreamedAudio streamedAudio)
{
//	this->streamedAudios.emplace(streamedAudio);
}

void Lilliputian::StreamedAudioSource2D::queueAudioToPlay(String streamedAudioName, uint8_t count)
{
	//if (this->streamedAudios.count(streamedAudioName) > 0 && count > 0)
	//{
	//	for (int i = 0; i < count; i++)
	//		this->audioPlayQueue.emplace(this->streamedAudios.at(streamedAudioName));
	//}
}

void Lilliputian::StreamedAudioSource2D::clearStreamedAudios()
{
//	this->streamedAudios.clear();
}

Lilliputian::Queue<Lilliputian::StreamedAudio> Lilliputian::StreamedAudioSource2D::popEntireAudioPlayQueue()
{
	Queue<StreamedAudio> outputQueue = this->audioPlayQueue;
	this->clearAudioPlayQueue();
	return outputQueue;
}

void Lilliputian::StreamedAudioSource2D::clearAudioPlayQueue()
{
	for (int i = 0; i < this->audioPlayQueue.size(); i++)
		this->audioPlayQueue.pop();
}


void Lilliputian::StreamedAudioSource2D::play(String streamedAudioName)
{

}

void Lilliputian::StreamedAudioSource2D::play()
{

}

void Lilliputian::StreamedAudioSource2D::pause()
{

}

void Lilliputian::StreamedAudioSource2D::stop()
{

}

Lilliputian::Vector<Lilliputian::String> Lilliputian::StreamedAudioSource2D::getStreamedAudioNames()
{
	Vector<String> streamedAudioNames;

	//for (Map<String, StreamedAudio>::iterator it = this->streamedAudios.begin();
	//	it != this->streamedAudios.end();
	//	++it)
	//{
	//	streamedAudioNames.push_back(it->first);
	//}

	return streamedAudioNames;
}

Lilliputian::StreamedAudio Lilliputian::StreamedAudioSource2D::getStreamedAudioByName(String streamedAudioName)
{
	StreamedAudio streamedAudio;

	//if (this->streamedAudios.count(streamedAudioName))
	//	streamedAudio = this->streamedAudios.at(streamedAudioName);

	return streamedAudio;
}