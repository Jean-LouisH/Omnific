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

#include "script_collection.hpp"
#include <asset_pipeline/asset_pipeline.hpp>
#include <asset_pipeline/assets/python_script.hpp>
#include <asset_pipeline/assets/cpp_script.hpp>

void Omnia::ScriptCollection::deserialize(YAML::Node yamlNode)
{
	for (YAML::const_iterator it3 = yamlNode.begin(); it3 != yamlNode.end(); ++it3)
	{
		if (it3->first.as<std::string>() == "python")
		{
			for (int i = 0; i < it3->second.size(); i++)
			{
				this->scripts.push_back(AssetPipeline::load<PythonScript>(it3->second[i].as<std::string>()));
			}
		}
		else if (it3->first.as<std::string>() == "cpp")
		{
			for (int i = 0; i < it3->second.size(); i++)
			{
				this->scripts.push_back(AssetPipeline::load<CPPScript>(it3->second[i].as<std::string>()));
			}
		}
	}
}