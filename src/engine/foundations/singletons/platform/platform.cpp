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

#include "platform.hpp"
#include <string>
#include "SDL.h"
#include "foundations/constants.hpp"
#include <thread>

uint64_t Omnific::Clock::get_delta()
{
	return this->delta;
}

float Omnific::Clock::get_delta_in_seconds()
{
	return this->delta / MS_IN_S;
}

void Omnific::Clock::set_start()
{
	this->start = SDL_GetTicks();
}

void Omnific::Clock::set_end()
{
	this->finish = SDL_GetTicks();
	this->delta = finish - start;
}

Omnific::Platform* Omnific::Platform::instance = nullptr;

void Omnific::Platform::initialize(
	int argc,
	char* argv[])
{
	Platform* new_instance = get_instance();

	std::vector<std::string> command_line_arguments;

	for (int i = 0; i < argc; i++)
		command_line_arguments.push_back(argv[i]);

	new_instance->shared_library_access = std::unique_ptr<SharedLibraryAccess>(new SharedLibraryAccess());
	new_instance->logger = std::unique_ptr<Logger>(new Logger());
	new_instance->input = std::unique_ptr<Input>(new Input());
	new_instance->file_access = std::unique_ptr<FileAccess>(new FileAccess(command_line_arguments[0]));
	new_instance->network_access = std::unique_ptr<NetworkAccess>(new NetworkAccess());
	new_instance->window = std::unique_ptr<Window>(new Window());
	new_instance->run_clock = std::unique_ptr<Clock>(new Clock());
	new_instance->run_clock->set_start();

	new_instance->command_line_arguments = command_line_arguments;
}

bool Omnific::Platform::create_window(std::string title,
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

void Omnific::Platform::yield_thread()
{
	std::this_thread::yield();
}

void Omnific::Platform::sleep_thread_for(int duration)
{
	if (duration > 0)
		SDL_Delay(duration);
}

void Omnific::Platform::show_error_box(std::string title, std::string message)
{
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
		title.c_str(),
		message.c_str(),
		NULL
	);
}

uint8_t Omnific::Platform::get_logical_core_count()
{
	return SDL_GetCPUCount();
}

uint32_t Omnific::Platform::get_l1_cache_line_size()
{
	return SDL_GetCPUCacheLineSize();
}

uint32_t Omnific::Platform::get_system_ram()
{
	return SDL_GetSystemRAM();
}

std::string Omnific::Platform::get_platform_name()
{
	return SDL_GetPlatform();
}

void Omnific::Platform::finalize()
{
	TTF_Quit();
	SDL_Quit();
	delete get_instance();
	instance = nullptr;
}

Omnific::SharedLibraryAccess& Omnific::Platform::get_shared_library_access()
{
	return *get_instance()->shared_library_access;
}

Omnific::Window& Omnific::Platform::get_window()
{
	return *get_instance()->window;
}

Omnific::Input& Omnific::Platform::get_input()
{
	return *get_instance()->input;
}

Omnific::Logger& Omnific::Platform::get_logger()
{
	return *get_instance()->logger;
}

Omnific::FileAccess& Omnific::Platform::get_file_access()
{
	return *get_instance()->file_access;
}

Omnific::NetworkAccess& Omnific::Platform::get_network_access()
{
	return *get_instance()->network_access;
}

Omnific::Clock& Omnific::Platform::get_run_clock()
{
	return *get_instance()->run_clock;
}

std::vector<std::string> Omnific::Platform::get_args()
{
	return get_instance()->command_line_arguments;
}

void Omnific::Platform::add_game_controller_mappings(std::string mapping_filepath)
{
	SDL_GameControllerAddMappingsFromFile(mapping_filepath.c_str());
}

Omnific::Platform* Omnific::Platform::get_instance()
{
	if (instance == nullptr)
		instance = new Platform();
	return instance;
}