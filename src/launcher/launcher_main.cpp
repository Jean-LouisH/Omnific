#include <stdlib.h>
#include <string>

#define APPLICATION_RELATIVE_DIRECTORY "bin"

#ifdef _WIN32
#define DIRECTORY_SLASHES "\\"
#else
#define DIRECTORY_SLASHES "/"
#endif

int main(int argc, char* argv[])
{
	std::string executable_filepath = argv[0];
	size_t directory_slash_index = executable_filepath.find_last_of(DIRECTORY_SLASHES);
	executable_filepath.insert(directory_slash_index + 1, APPLICATION_RELATIVE_DIRECTORY);
	std::string arg_strings;
	for (int i = 1; i < argc; i++)
	{
		arg_strings += " ";
		arg_strings += argv[i];
	}
	system((executable_filepath + arg_strings).c_str());
	return 0;
}