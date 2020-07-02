#include "FileSystem.hpp"
#include <SDL.h>

Lilliputian::FileSystem::FileSystem(const char* filepath)
{
	this->setFilePath(filepath);
}

void Lilliputian::FileSystem::setFilePath(const char* filepath)
{
	this->filepath = filepath;
}

Lilliputian::String Lilliputian::FileSystem::getFilePath()
{
	return this->filepath;
}

Lilliputian::String Lilliputian::FileSystem::getExecutableName()
{
	String executableName;
	int nameIndexStart = 0;

	//Find the first slash from the end
	for (nameIndexStart = this->filepath.size() - 1;
		this->filepath.at(nameIndexStart) != '/' && this->filepath.at(nameIndexStart) != '\\';
		nameIndexStart--)
		;

	//append from the start index to the extension name.
	for (int j = nameIndexStart + 1; this->filepath.at(j) != '.'; j++)
		executableName += this->filepath.at(j);

	return executableName;
}