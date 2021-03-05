#include "audio_stream_source_2d.hpp"

void Lilliputian::AudioStreamSource2D::addAudioStream(AudioStream audioStream)
{
//	this->audioStreams.emplace(audioStream);
}

void Lilliputian::AudioStreamSource2D::queueAudioToPlay(String audioStreamName, uint8_t count)
{
	//if (this->audioStreams.count(audioStreamName) > 0 && count > 0)
	//{
	//	for (int i = 0; i < count; i++)
	//		this->audioPlayQueue.emplace(this->audioStreams.at(audioStreamName));
	//}
}

void Lilliputian::AudioStreamSource2D::clearAudioStreams()
{
//	this->audioStreams.clear();
}

Lilliputian::Queue<Lilliputian::AudioStream> Lilliputian::AudioStreamSource2D::popEntireAudioPlayQueue()
{
	Queue<AudioStream> outputQueue = this->audioPlayQueue;
	this->clearAudioPlayQueue();
	return outputQueue;
}

void Lilliputian::AudioStreamSource2D::clearAudioPlayQueue()
{
	for (int i = 0; i < this->audioPlayQueue.size(); i++)
		this->audioPlayQueue.pop();
}


void Lilliputian::AudioStreamSource2D::play(String audioStreamName)
{

}

void Lilliputian::AudioStreamSource2D::play()
{

}

void Lilliputian::AudioStreamSource2D::pause()
{

}

void Lilliputian::AudioStreamSource2D::stop()
{

}

Lilliputian::Vector<Lilliputian::String> Lilliputian::AudioStreamSource2D::getAudioStreamNames()
{
	Vector<String> audioStreamNames;

	//for (Map<String, AudioStream>::iterator it = this->audioStreams.begin();
	//	it != this->audioStreams.end();
	//	++it)
	//{
	//	audioStreamNames.push_back(it->first);
	//}

	return audioStreamNames;
}

Lilliputian::AudioStream Lilliputian::AudioStreamSource2D::getAudioStreamByName(String audioStreamName)
{
	AudioStream audioStream;

	//if (this->audioStreams.count(audioStreamName))
	//	audioStream = this->audioStreams.at(audioStreamName);

	return audioStream;
}