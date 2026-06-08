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

#include "random_number_generator.hpp"

Omnific::RandomNumberGenerator* Omnific::RandomNumberGenerator::instance = nullptr;

double Omnific::RandomNumberGenerator::get_random_float(double a, double b) 
{
	RandomNumberGenerator* instance = RandomNumberGenerator::get_instance();
    std::uniform_real_distribution<double> distribution(a, b);
    return distribution(instance->random_engine);
}

int Omnific::RandomNumberGenerator::get_random_integer(int a, int b) 
{
	RandomNumberGenerator* instance = RandomNumberGenerator::get_instance();
    std::uniform_int_distribution<int> distribution(a, b);
    return distribution(instance->random_engine);
}

bool Omnific::RandomNumberGenerator::get_random_boolean()
{
	RandomNumberGenerator* instance = RandomNumberGenerator::get_instance();
    std::bernoulli_distribution distribution(0.5);
    return distribution(instance->random_engine);
}

Omnific::RandomNumberGenerator* Omnific::RandomNumberGenerator::get_instance()
{
	if (instance == nullptr)
	{
		instance = new RandomNumberGenerator();
		instance->random_engine.seed(std::random_device{}());
	}
	return instance;
}