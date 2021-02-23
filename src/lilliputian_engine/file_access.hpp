#pragma once

#include "utilities/string.hpp"

namespace Lilliputian
{
	class FileAccess
	{
	public:
		FileAccess(const char* executableFilepath);
		String getFilePath();
		String getExecutableName();
		bool exists(String filepath);
	private:
		String executableFilepath;
	};
}