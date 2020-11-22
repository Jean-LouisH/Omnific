#include "FileAccess.hpp"
#include <SDL.h>
#include <fstream>

Lilliputian::FileAccess::FileAccess(const char* executableFilepath)
{
	this->executableFilepath = executableFilepath;
}

Lilliputian::String Lilliputian::FileAccess::getFilePath()
{
	return this->executableFilepath;
}

Lilliputian::String Lilliputian::FileAccess::getExecutableName()
{
	String executableName;
	int nameIndexStart = 0;

	//Find the first slash from the end
	for (nameIndexStart = this->executableFilepath.size() - 1;
		this->executableFilepath.at(nameIndexStart) != '/' && this->executableFilepath.at(nameIndexStart) != '\\';
		nameIndexStart--)
		;

	//append from the start index to the extension name.
	for (int j = nameIndexStart + 1; this->executableFilepath.at(j) != '.'; j++)
		executableName += this->executableFilepath.at(j);

	return executableName;
}

bool Lilliputian::FileAccess::exists(String filepath)
{
	std::ifstream inputFile(filepath);
	return inputFile.good();
}