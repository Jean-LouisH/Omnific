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

#include "haptic_signal_buffer.hpp"


void Omnific::HapticSignalBuffer::publish(PlayerID player_id, float strength_pct, uint16_t duration_ms)
{
	std::queue<HapticSignal> haptic_signal_queue;
	HapticSignal new_haptic_signal = HapticSignal(player_id, strength_pct, duration_ms);

	if (this->haptic_signals.count(player_id) == 0)
	{
		haptic_signal_queue.push(new_haptic_signal);
		this->haptic_signals.emplace(player_id, haptic_signal_queue);
	}
	else
	{
		haptic_signal_queue = this->haptic_signals.at(player_id);
		haptic_signal_queue.push(new_haptic_signal);
		this->haptic_signals.at(player_id) = haptic_signal_queue;
	}

}

void Omnific::HapticSignalBuffer::clear()
{
	this->haptic_signals.clear();
}

std::unordered_map<Omnific::PlayerID, std::queue<Omnific::HapticSignal>>& Omnific::HapticSignalBuffer::get_haptic_signals()
{
	return this->haptic_signals;
}

std::queue<Omnific::HapticSignal>& Omnific::HapticSignalBuffer::query(PlayerID player_id)
{
	return this->haptic_signals.at(player_id);
}