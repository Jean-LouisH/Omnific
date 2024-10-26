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
	size_t directory_slash_index = executable_filepath.find_last_of("\\");
	executable_filepath.insert(directory_slash_index + 1, application_relative_directory);
	std::string arg_strings;

	for (int i = 1; i < argc; i++)
	{
		arg_strings += " ";
		arg_strings += argv[i];
	}

	if (executable_filepath.size() > 0)
		executable_filepath = "\"" + executable_filepath + "\"";

	std::string command_string;

	if (arg_strings.size() > 0)
	{
		arg_strings = "\"" + arg_strings + "\"";
		command_string = ("\"" + executable_filepath + arg_strings + "\"");
	}
	else
	{
		command_string = executable_filepath;
	}
	
	system(command_string.c_str());
	return 0;
}
