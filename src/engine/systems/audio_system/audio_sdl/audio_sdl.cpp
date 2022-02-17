// MIT License
// 
// Copyright (c) 2020 Jean-Louis Haywood
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "audio_sdl.hpp"

void SDL::Audio::playSoundFXs(std::queue<std::shared_ptr<Mix_Chunk>>* soundFXQueue)
{
	while (!soundFXQueue->empty())
	{
		Mix_PlayChannel(-1, soundFXQueue->front().get(), 0);
		soundFXQueue->pop();
	}
}

void SDL::Audio::playMusic(std::queue<std::shared_ptr<Mix_Music>>* musicQueue)
{
	/* For now plays the last music that was queued. */
	while (!musicQueue->empty())
	{
		Mix_PlayMusic(musicQueue->front().get(), -1);
		musicQueue->pop();
	}
}

void SDL::Audio::stopMusic()
{
	Mix_HaltMusic();
}