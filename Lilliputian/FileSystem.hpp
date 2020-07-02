#pragma once

#include "Utilities/String.hpp"

namespace Lilliputian
{
	class FileSystem
	{
	private:
		String filepath;
	public:
		FileSystem(const char* filepath);
		void setFilePath(const char* filepath);
		String getFilePath();
		String getExecutableName();
	};
}