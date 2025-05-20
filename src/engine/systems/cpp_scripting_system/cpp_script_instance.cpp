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

#include "cpp_script_instance.hpp"

void Omnific::CPPScriptInstance::on_entity_start()
{
    this->has_on_entity_start_method = false;
}

void Omnific::CPPScriptInstance::on_input()
{
    this->has_on_input_method = false;
}

void Omnific::CPPScriptInstance::on_early_update()
{
    this->has_on_early_update_method = false;
}

void Omnific::CPPScriptInstance::on_update()
{
    this->has_on_update_method = false;
}

void Omnific::CPPScriptInstance::on_fixed_update()
{
    this->has_on_fixed_update_method = false;
}

void Omnific::CPPScriptInstance::on_late_update()
{
    this->has_on_late_update_method = false;
}

void Omnific::CPPScriptInstance::on_output()
{
    this->has_on_output_method = false;
}

void Omnific::CPPScriptInstance::on_entity_finish()
{
    this->has_on_entity_finish_method = false;
}