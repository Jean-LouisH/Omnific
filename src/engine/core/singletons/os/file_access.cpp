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
#include <core/singletons/os/os.hpp>

Omnia::FileAccess::FileAccess(std::string executableFilepath)
{
	this->executableFilepath = executableFilepath;
}

Omnia::FileAccess::~FileAccess()
{
	for (std::thread* fileLoadingThread : this->ioThreads)
		fileLoadingThread->join();
}

void Omnia::FileAccess::setDataDirectory(std::string assetDirectory)
{
	this->dataDirectory = assetDirectory;
	OS::getLogger().write("Data Directory set to \"" + assetDirectory + "\"");
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
	int nameIndexStart = filepath.size() - 1;

	if (nameIndexStart > -1)
	{
		//Find the first slash from the end
		while (filepath.at(nameIndexStart) != '/' && filepath.at(nameIndexStart) != '\\')
		{
			nameIndexStart--;
			if (nameIndexStart < 0)
			{
				break;
			}
		}
	}

	//append from the start index to the extension name.
	for (int j = nameIndexStart + 1; filepath.at(j) != '.';	j++)
		fileName += filepath.at(j);

	return fileName;
}

std::string Omnia::FileAccess::getFileExtension(std::string filepath)
{
	std::string fileName;
	int nameIndexStart = 0;

	//Find the first slash from the end
	for (nameIndexStart = filepath.size() - 1;
		filepath.at(nameIndexStart) != '.';
		nameIndexStart--)
		;

	//append from the start index to the extension name.
	for (int j = nameIndexStart + 1; filepath.at(j) != '\0'; j++)
		fileName += filepath.at(j);

	return fileName;
}

std::string Omnia::FileAccess::getPathBeforeFile(std::string filepath)
{
	std::string path;
	int nameEndIndex = filepath.size() - 1;

	//Find the first slash from the end
	if (nameEndIndex > -1)
	{
		while ((filepath.at(nameEndIndex) != '/' && filepath.at(nameEndIndex) != '\\'))
		{
			nameEndIndex--;
			if (nameEndIndex < 0)
			{
				break;
			}
		}
	}

	if (nameEndIndex > -1)
	{
		//append from the start index to the extension name.
		for (int j = 0; j != nameEndIndex; j++)
		{
			path += filepath.at(j);
		}
	}

	return path;
}

bool Omnia::FileAccess::exists(std::string filepath)
{
	std::ifstream inputFile(filepath);
	return inputFile.good();
}

std::string Omnia::FileAccess::readString(std::string filepath, bool applyDataDirectory)
{
	std::string outputString;
	std::vector<uint8_t> binary = this->readBinary(filepath, applyDataDirectory);
	outputString.assign(binary.begin(), binary.end());
	return outputString;
}

std::vector<uint8_t> Omnia::FileAccess::readBinary(std::string filepath, bool applyDataDirectory)
{
	std::vector<uint8_t> outputBinary;
	std::string fullFilepath = this->getFilepathWithDataDirectory(filepath, applyDataDirectory);
	std::FILE* filePointer = std::fopen(fullFilepath.c_str(), "rb");

	if (filePointer)
	{
		std::fseek(filePointer, 0, SEEK_END);
		outputBinary.resize(std::ftell(filePointer));
		std::rewind(filePointer);
		std::fread(&outputBinary[0], 1, outputBinary.size(), filePointer);
		std::fclose(filePointer);
	}

	return outputBinary;
}

void Omnia::FileAccess::writeBinary(std::string filepath, std::vector<uint8_t> binary, bool applyDataDirectory)
{

}

void Omnia::FileAccess::requestAsynchronousBinaryReading(std::string filepath, bool applyDataDirectory)
{
	std::string fullFilepath = this->getFilepathWithDataDirectory(filepath, applyDataDirectory);

	if (!this->asynchronouslyLoadedBinaries.count(fullFilepath))
		this->ioThreads.push_back(new std::thread(&FileAccess::readBinaryAsynchronously, this, filepath, applyDataDirectory));
}

void Omnia::FileAccess::requestAsynchronousBinaryWriting(std::string filepath, std::vector<uint8_t> binary, bool applyDataDirectory)
{
	std::string fullFilepath = this->getFilepathWithDataDirectory(filepath, applyDataDirectory);
	this->ioThreads.push_back(new std::thread(&FileAccess::writeBinaryAsynchronously, this, filepath, binary, applyDataDirectory));
}

bool Omnia::FileAccess::hasBinaryBeenReadAsynchronously(std::string filepath, bool applyDataDirectory)
{
	std::string fullFilepath = this->getFilepathWithDataDirectory(filepath, applyDataDirectory);
	return this->asynchronouslyLoadedBinaries.count(fullFilepath);
}

std::vector<uint8_t> Omnia::FileAccess::fetchAsynchronouslyReadBinary(std::string filepath, bool applyDataDirectory)
{
	std::string fullFilepath = this->getFilepathWithDataDirectory(filepath, applyDataDirectory);
	std::vector<uint8_t> binary;
	if (this->asynchronouslyLoadedBinaries.count(fullFilepath))
		binary = this->asynchronouslyLoadedBinaries.at(fullFilepath);
	return binary;
}

std::string Omnia::FileAccess::getFilepathWithDataDirectory(std::string filepath, bool applyDataDirectory)
{
	std::string fullFilepath;

	if (applyDataDirectory)
		fullFilepath = this->getDataDirectoryPath() + filepath;
	else
		fullFilepath = filepath;

	return fullFilepath;
}

void Omnia::FileAccess::readBinaryAsynchronously(std::string filepath, bool applyDataDirectory)
{
	std::vector<uint8_t> binary = this->readBinary(filepath, applyDataDirectory);
	this->asynchronouslyLoadedBinaries.emplace(filepath, binary);
}

void Omnia::FileAccess::writeBinaryAsynchronously(std::string filepath, std::vector<uint8_t> binary, bool applyDataDirectory)
{
	this->writeBinary(filepath, binary, applyDataDirectory);
}