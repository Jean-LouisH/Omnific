#pragma once

#include "Utilities/String.hpp"

namespace Lilliputian
{
	class FileAccess
	{
	private:
		String executableFilepath;
	public:
		FileAccess(const char* executableFilepath);
		String getFilePath();
		String getExecutableName();
		bool exists(String filepath);
	};
}