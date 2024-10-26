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

#include "registerable.hpp"
#include <foundations/singletons/uid_generator.hpp>

Omnific::Registerable::Registerable()
{
	this->id = UIDGenerator::get_new_uid();
}

Omnific::Registerable* Omnific::Registerable::instance()
{
	Registerable* clone = new Registerable(*this);
	clone->id = UIDGenerator::get_new_uid();
	return clone;
}

std::string Omnific::Registerable::get_type()
{
	return this->type;
}

Omnific::UID Omnific::Registerable::get_id()
{
	return this->id;
}

bool Omnific::Registerable::is_type(std::string type_string)
{
	return this->type == type_string;
}