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

#include "audio_synthesis.hpp"

Omnia::AudioSynthesis::~AudioSynthesis()
{
	lb_freeComposition(this->composition);
	this->composition = nullptr;
}

Omnia::AudioSynthesis::AudioSynthesis(std::string filepath)
{
	this->composition = lb_createComposition(filepath.c_str());
	this->set_name(filepath);
	this->type = TYPE_STRING;
}

float Omnia::AudioSynthesis::get_playback_length()
{
	return this->composition->timeLength;
}

std::vector<uint16_t> Omnia::AudioSynthesis::get_spectrum_data()
{
	std::vector<uint16_t> spectrum;
	lb_BinaryS16 binary = lb_getSpectrumData(this->composition);

	for (int i = 0; i < binary.size; i++)
	{
		spectrum.push_back(binary.data[i]);
	}

	return spectrum;
}

lb_Composition* Omnia::AudioSynthesis::get_composition()
{
	return this->composition;
}