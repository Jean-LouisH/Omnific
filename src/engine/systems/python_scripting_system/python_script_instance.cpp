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

#include "python_script_instance.hpp"

void Omnific::PythonScriptInstance::set_data(pybind11::object new_object)
{
	this->data = new_object;
}

void Omnific::PythonScriptInstance::set_callable(std::string method_name)
{
	this->callable_methods.emplace(method_name);
}

pybind11::object Omnific::PythonScriptInstance::test(std::string method_name)
{
	return this->data.attr(method_name.c_str());
}

void Omnific::PythonScriptInstance::call(std::string method_name)
{
	this->test(method_name)();
}

bool Omnific::PythonScriptInstance::has_callable(std::string method_name)
{
	return this->callable_methods.count(method_name) > 0;
}
