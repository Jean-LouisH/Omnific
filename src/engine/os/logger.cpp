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

#include "logger.hpp"
#include <iostream>
#include <chrono>
#include <ctime>
#include <fstream>

void Esi::Logger::write(std::string message)
{
	std::string timeStampedMessage = this->timeStamp(message);
	std::cout << std::endl << timeStampedMessage;
	this->logs.push_back(timeStampedMessage);
}

void Esi::Logger::writeToFile(std::string message)
{
	std::string timeStampedMessage = this->timeStamp(message);



	this->logs.push_back(timeStampedMessage);
}

std::string Esi::Logger::getLastMessage()
{
	return this->logs.at(this->logs.size() - 1);
}

std::vector<std::string> Esi::Logger::getLogs()
{
	return this->logs;
}

std::string Esi::Logger::timeStamp(std::string message)
{
	auto now = std::chrono::system_clock::now();
	std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
	std::string result = std::ctime(&currentTime);
	return 	"[" + result.substr(0, result.find("\n")) + "]: " + message;
}