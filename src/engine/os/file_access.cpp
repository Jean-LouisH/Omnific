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

#include "file_access.hpp"
#include <SDL.h>
#include <fstream>
#include <algorithm>

Omnia::FileAccess::FileAccess(std::string executableFilepath)
{
	this->executableFilepath = executableFilepath;
}

void Omnia::FileAccess::setDataDirectory(std::string assetDirectory)
{
	this->dataDirectory = assetDirectory;
}

std::shared_ptr<Omnia::Scene> Omnia::FileAccess::loadScene(std::string filepath)
{
	std::shared_ptr<Scene> scene;
	return scene;
}

std::string Omnia::FileAccess::getExecutableFilePath()
{
	return this->executableFilepath;
}

std::string Omnia::FileAccess::getExecutableName()
{
	return this->getFileNameWithoutExtension(this->executableFilepath);
}

std::string Omnia::FileAccess::getExecutableDirectoryPath()
{
	return this->getPathBeforeFile(this->executableFilepath);
}

std::string Omnia::FileAccess::getDataDirectoryPath()
{
	return this->dataDirectory;
}

std::string Omnia::FileAccess::getFileNameWithoutExtension(std::string filepath)
{
	std::string fileName;
	int nameIndexStart = 0;

	//Find the first slash from the end
	for (nameIndexStart = filepath.size() - 1;
		filepath.at(nameIndexStart) != '/' && filepath.at(nameIndexStart) != '\\';
		nameIndexStart--)
		;

	//append from the start index to the extension name.
	for (int j = nameIndexStart + 1; filepath.at(j) != '.';	j++)
		fileName += filepath.at(j);

	return fileName;
}

std::string Omnia::FileAccess::getPathBeforeFile(std::string filepath)
{
	std::string path;
	int nameEndIndex = 0;

	//Find the first slash from the end
	for (nameEndIndex = filepath.size() - 1;
		filepath.at(nameEndIndex) != '/' && filepath.at(nameEndIndex) != '\\';
		nameEndIndex--)
		;

	//append from the start index to the extension name.
	for (int j = 0; j != nameEndIndex; j++)
		path += filepath.at(j);

	return path;
}

bool Omnia::FileAccess::exists(std::string filepath)
{
	std::ifstream inputFile(filepath);
	return inputFile.good();
}