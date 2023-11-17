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

#include "engine.hpp"
#include <thread>
#include "core/utilities/constants.hpp"
#include "core/assets/image.hpp"
#include <iostream>

#include <core/singletons/event_bus.hpp>

#include <customization/class_registry/class_registry.hpp>

void Omnia::Engine::run(
	int argc,
	char* argv[])
{
	OS::initialize(argc, argv);
	Logger& logger = OS::get_logger();
	logger.write("Initializing Omnia Engine...");
	ClassRegistry::initialize();

	do
	{
		Profiler& profiler = OS::get_profiler();

		this->initialize();

		if (this->state != State::FINALIZING)
		{

			/* These timers persist throughout Engine runtime and
			   keep track of elapsed times in nanoseconds. */
			profiler.add_timer(MAIN_THREAD_TIMER_NAME, false);
			profiler.add_timer(LOOP_THREAD_TIMER_NAME, false);

			/* Engine threading uses a hybrid of dedicated threads
			   for deadline sensitive tasks and a thread pool for
			   general parallelizable tasks. */

			std::vector<std::thread> dedicated_threads;
			dedicated_threads.push_back(std::thread(&Engine::run_loop, this, profiler.get_timer(LOOP_THREAD_TIMER_NAME)));

			OS::get_thread_pool().initialize();

			logger.write("Engine loops currently running...");

			/* Input loop must run on the main thread. */
			Input& input = OS::get_input();
			std::shared_ptr<HiResTimer> input_process_timer = profiler.get_timer(MAIN_THREAD_TIMER_NAME);

			while (this->state == State::RUNNING)
			{
				input_process_timer->set_start();
				input.detect_game_controllers();
				input.poll_input_events();
				if (input.has_requested_shutdown())
					this->state = State::FINALIZING;
				if (input.has_requested_restart())
					this->state = State::RESTARTING;
				input_process_timer->set_end();
				this->sleep_this_thread_for_remaining_time(INPUT_TARGET_FPS, input_process_timer);
			}

			logger.write("Finalizing Engine loops...");
			for (std::thread& thread : dedicated_threads)
				thread.join();

			this->finalize();
		}
	} while (this->state == State::RESTARTING);
}

void Omnia::Engine::initialize()
{
	Logger& logger = OS::get_logger();

	if (this->state != State::RESTARTING)
		this->state = State::INITIALIZING;

	logger.write("Loading Systems from ClassRegistry...");

	/* Load Systems from the ClassRegistry */
	for (auto it : ClassRegistry::query_all<System>())
	{
		std::shared_ptr<System> system = std::dynamic_pointer_cast<System>(it.second);

		this->systems.emplace(
			it.first,
			std::dynamic_pointer_cast<System>(std::shared_ptr<Registerable>(system->instance())));
	}

	logger.write("Querying Platform...");

	Platform& platform = OS::get_platform();

	logger.write("Retrieved Logical Core Count: " + std::to_string(platform.get_logical_core_count()));
	logger.write("Retrieved L1 Cache Line Size: " + std::to_string(platform.get_l1cache_line_size()) + " B");
	logger.write("Retrieved OS Name: \"" + platform.get_osname() + "\"");
	logger.write("Retrieved System RAM: " + std::to_string(platform.get_system_ram()) + " MB");

	std::string data_directory = DATA_DIRECTORY;

#ifdef _DEBUG
	while (true)
	{
		OS::get_window().hide();
		std::cout << "\n\nChoose data project to load:" << std::endl;
		std::cout << "0. Custom" << std::endl;
		std::cout << "1. Demos" << std::endl;
		std::cout << "2. Editor" << std::endl;
		std::cout << "\n-> #";

		std::string input_string;
		std::cin >> input_string;
		OS::get_window().show();

		if (input_string == "0")
		{
			break;
		}
		else if (input_string == "1")
		{
			data_directory = DEBUG_DEMO_DATA_DIRECTORY;
			break;
		}
		else if (input_string == "2")
		{
			data_directory = DEBUG_EDITOR_DATA_DIRECTORY;
			break;
		}
		else
		{
			std::cout << "Error: Option \"" + input_string + "\" not listed." << std::endl;
		}
	}
#endif
	std::string boot_filepath = data_directory + BOOT_FILE_NAME;
	FileAccess& file_access = OS::get_file_access();

	if (file_access.exists(boot_filepath))
	{
		Configuration::load_from_file(boot_filepath);
		OS::get_file_access().set_data_directory(data_directory);
	}

#ifdef DEBUG_CONSOLE_ENABLED
	std::cout << "\n\nPress '`' in-application to write to command line via console.";
	std::cout << "\n\n";
#endif

	if (Configuration::get_instance()->is_loaded)
	{
		OS::add_game_controller_mappings(data_directory + GAME_CONTROLLER_DATABASE_FILE_NAME);
		Window& window = OS::get_window();
		window.resize(Configuration::get_instance()->window_settings.width, Configuration::get_instance()->window_settings.height);
		window.change_title(Configuration::get_instance()->metadata.title.c_str());
		this->state = State::RUNNING;
		logger.write("Loaded application project \"" + Configuration::get_instance()->metadata.title + "\" at: " + data_directory);
	}
	else
	{
		OS::show_error_box(
			"Could not load game data",
			"The game data is either missing or corrupted. Reinstall and try again");
		logger.write("Shutting down Engine due to error in loading Configuration.");
		this->state = State::FINALIZING;
	}

}

void Omnia::Engine::run_loop(std::shared_ptr<HiResTimer> loop_process_timer)
{
	for (auto system : this->systems)
		system.second->initialize();

	FileAccess& file_access = OS::get_file_access();
	std::string entry_scene_filepath = Configuration::get_instance()->metadata.entry_scene_filepath;

	if (file_access.exists(file_access.get_data_directory_path() + entry_scene_filepath))
		SceneStorage::load_scene(std::shared_ptr<Scene>(new Scene(entry_scene_filepath)));

	Profiler& profiler = OS::get_profiler();
	profiler.add_timer(LOOP_FRAME_TIMER_NAME);
	std::shared_ptr<HiResTimer> loop_frame_timer = profiler.get_timer(LOOP_FRAME_TIMER_NAME);

	while (this->state == State::RUNNING)
	{
		loop_process_timer->set_start();
		loop_frame_timer->set_start();

		std::shared_ptr<Scene> active_scene = SceneStorage::get_active_scene();
		uint32_t ms_per_compute_update = Configuration::get_instance()->time_settings.ms_per_compute_update;

		if (ms_per_compute_update > MAXIMUM_MS_PER_COMPUTE_UPDATE)
			ms_per_compute_update = MAXIMUM_MS_PER_COMPUTE_UPDATE;

		if (OS::get_input().get_has_detected_input_changes())
			for (auto system : this->systems)
				system.second->on_input(active_scene);

		for (auto system : this->systems)
			system.second->on_start(active_scene);

		for (auto it : active_scene->get_scene_layers())
			it.second->clear_start_entity_queue();

		for (auto system : this->systems)
			system.second->on_early(active_scene);

		for (auto system : this->systems)
			system.second->on_logic(active_scene);

		/* This calls the compute based Systems repeatedly until the accumulated
		   lag milliseconds are depleted. This ensures compute operations
		   are accurate to real-time, even when frames drop. */

		while (profiler.get_lag_count() >= ms_per_compute_update && 
			this->state == State::RUNNING)
		{
			for (auto system : this->systems)
				system.second->on_compute(active_scene);
			profiler.decrement_lag_count(ms_per_compute_update);
		}

		for (auto system : this->systems)
			system.second->on_late(active_scene);

		for (auto system : this->systems)
			system.second->on_finish(active_scene);

		for (auto it : active_scene->get_scene_layers())
			it.second->clear_finish_entity_queue();

		EventBus::clear();

		profiler.increment_lag_count(loop_frame_timer->get_delta());
		loop_process_timer->set_end();

		this->sleep_this_thread_for_remaining_time(
			UPDATE_TARGET_FPS,
			loop_process_timer);
		loop_frame_timer->set_end();
	}

	for (auto system : this->systems)
		system.second->finalize();
}

void Omnia::Engine::sleep_this_thread_for_remaining_time(uint32_t target_fps, std::shared_ptr<HiResTimer> run_timer)
{
	float target_frame_time = 1000.0 / target_fps;
	float run_time = run_timer->get_delta();
	OS::sleep_this_thread_for(target_frame_time - run_time);
}

void Omnia::Engine::finalize()
{
	SceneStorage::clear_scenes();
	OS::get_thread_pool().finalize();
}