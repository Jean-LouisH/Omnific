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

#include "os.hpp"
#include <string>
#include "SDL.h"
#include "core/utilities/constants.hpp"
#include <thread>

Omnia::OS* Omnia::OS::instance = nullptr;

void Omnia::OS::initialize(
	int argc,
	char* argv[]
)
{
	OS* new_instance = get_instance();

	std::vector<std::string> command_line_arguments;

	for (int i = 0; i < argc; i++)
		command_line_arguments.push_back(argv[i]);

	new_instance->shared_library_access = std::unique_ptr<SharedLibraryAccess>(new SharedLibraryAccess());
	new_instance->logger = std::unique_ptr<Logger>(new Logger());
	new_instance->input = std::unique_ptr<Input>(new Input());
	new_instance->file_access = std::unique_ptr<FileAccess>(new FileAccess(command_line_arguments[0]));
	new_instance->network_access = std::unique_ptr<NetworkAccess>(new NetworkAccess());
	new_instance->profiler = std::unique_ptr<Profiler>(new Profiler());
	new_instance->platform = std::unique_ptr<Platform>(new Platform());
	new_instance->thread_pool = std::unique_ptr<ThreadPool>(new ThreadPool());
	new_instance->window = std::unique_ptr<Window>(new Window());
	new_instance->run_timer = std::unique_ptr<HiResTimer>(new HiResTimer());
	new_instance->run_timer->set_start();

	new_instance->command_line_arguments = command_line_arguments;
}

bool Omnia::OS::create_window(std::string title,
	uint16_t width,
	uint16_t height,
	bool is_fullscreen,
	std::string rendering_context)
{
	bool is_successful = !(bool)SDL_Init(SDL_INIT_EVERYTHING);

	if (is_successful)
	{
		get_instance()->window->initialize(title, width, height, is_fullscreen, rendering_context);

		if (TTF_Init() == -1)
			printf("TTF_Init: %s\n", TTF_GetError());
	}
	else
	{
		SDL_Log(
			"SDL could not initialize because: %s",
			SDL_GetError);
	}

	return is_successful;
}

void Omnia::OS::yield_this_thread()
{
	std::this_thread::yield();
}

void Omnia::OS::sleep_this_thread_for(int duration)
{
	if (duration > 0)
		std::this_thread::sleep_for(std::chrono::milliseconds(duration));
}

void Omnia::OS::show_error_box(std::string title, std::string message)
{
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
		title.c_str(),
		message.c_str(),
		NULL
	);
}

void Omnia::OS::finalize()
{
	TTF_Quit();
	SDL_Quit();
	get_instance()->get_thread_pool().finalize();
	delete get_instance();
	instance = nullptr;
}

Omnia::SharedLibraryAccess& Omnia::OS::get_shared_library_access()
{
	return *get_instance()->shared_library_access;
}

Omnia::Window& Omnia::OS::get_window()
{
	return *get_instance()->window;
}

Omnia::Input& Omnia::OS::get_input()
{
	return *get_instance()->input;
}

Omnia::Logger& Omnia::OS::get_logger()
{
	return *get_instance()->logger;
}

Omnia::FileAccess& Omnia::OS::get_file_access()
{
	return *get_instance()->file_access;
}

Omnia::NetworkAccess& Omnia::OS::get_network_access()
{
	return *get_instance()->network_access;
}

Omnia::Profiler& Omnia::OS::get_profiler()
{
	return *get_instance()->profiler;
}

Omnia::Platform& Omnia::OS::get_platform()
{
	return *get_instance()->platform;
}

Omnia::ThreadPool& Omnia::OS::get_thread_pool()
{
	return *get_instance()->thread_pool;
}

Omnia::HiResTimer& Omnia::OS::get_run_timer()
{
	return *get_instance()->run_timer;
}

std::vector<std::string> Omnia::OS::get_args()
{
	return get_instance()->command_line_arguments;
}

void Omnia::OS::add_game_controller_mappings(std::string mapping_filepath)
{
	SDL_GameControllerAddMappingsFromFile(mapping_filepath.c_str());
}

Omnia::OS* Omnia::OS::get_instance()
{
	if (instance == nullptr)
		instance = new OS();
	return instance;
}