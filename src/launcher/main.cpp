#include <stdlib.h>
#include <string>

int main(int argc, char* argv[])
{
#ifdef _WIN32
	std::string application_relative_directory = "bin\\";
#else
	std::string application_relative_directory = "bin/";
#endif

	std::string executable_filepath = argv[0];
#ifdef _WIN32
	size_t directory_slash_index = executable_filepath.find_last_of("\\");
#else
	size_t directory_slash_index = executable_filepath.find_last_of("/");
#endif
	executable_filepath.insert(directory_slash_index + 1, application_relative_directory);
	std::string arg_strings;

	for (int i = 1; i < argc; ++i)
	{
		arg_strings += " ";
		arg_strings += argv[i];
	}

	std::string command_string;
	command_string = (executable_filepath + arg_strings);

	system(command_string.c_str());
	return 0;
}
