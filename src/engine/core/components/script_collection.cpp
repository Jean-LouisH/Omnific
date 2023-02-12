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
#include <core/scene/scene.hpp>
#include <core/singletons/os/os.hpp>
#include <core/assets/script.hpp>

void Omnia::ScriptCollection::deserialize(YAML::Node yamlNode)
{
	for (YAML::const_iterator it3 = yamlNode.begin(); it3 != yamlNode.end(); ++it3)
	{
		std::string languageName = it3->first.as<std::string>();

		if (languageName == "Python")
		{
			for (int i = 0; i < it3->second.size(); i++)
			{
				std::shared_ptr<Script> pythonScript(OS::getFileAccess().loadAssetByType<Script>(it3->second[i].as<std::string>(), false));
				pythonScript->setLanguageName(languageName);
				this->scripts.push_back(pythonScript);
			}
		}
		else if (languageName == "CPP")
		{
			for (int i = 0; i < it3->second.size(); i++)
			{
				std::shared_ptr<Script> cppScript(OS::getFileAccess().loadAssetByType<Script>(it3->second[i].as<std::string>()));
				cppScript->setLanguageName(languageName);
				this->scripts.push_back(cppScript);
			}
		}
	}
}