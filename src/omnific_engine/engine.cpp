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
#include "foundations/constants.hpp"
#include "foundations/resources/image.hpp"
#include <iostream>
#include "foundations/singletons/profiler.hpp"
#include "foundations/singletons/thread_pool.hpp"
#include <foundations/singletons/event_bus.hpp>
#include <customization/class_registry.hpp>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

void Omnific::Engine::set_callback_functions(
	void (*load_script_instances)(),
	void (*on_input)(),
	void (*on_start)(),
	void (*on_early)(),
	void (*on_logic)(),
	void (*on_compute)(),
	void (*on_late)(),
	void (*on_finish)(),
	void (*on_output)())
{
	this->application_callback_functions.load_script_instances = load_script_instances;
	this->application_callback_functions.on_input = on_input;
	this->application_callback_functions.on_start = on_start;
	this->application_callback_functions.on_early = on_early;
	this->application_callback_functions.on_logic = on_logic;
	this->application_callback_functions.on_compute = on_compute;
	this->application_callback_functions.on_late = on_late;
	this->application_callback_functions.on_finish = on_finish;
	this->application_callback_functions.on_output = on_output;
}

void Omnific::Engine::add_app_data_directory(std::string app_data_directory)
{
	this->app_data_directories.push_back(app_data_directory);
}

void Omnific::Engine::add_app_data_directories(std::vector<std::string> app_data_directories)
{
	this->app_data_directories.insert(
		this->app_data_directories.end(), 
		app_data_directories.begin(), 
		app_data_directories.end()
	);
}


void Omnific::Engine::run(
	int argc,
	char* argv[])
{
	Platform::initialize(argc, argv);
	Logger& logger = Platform::get_logger();
	ClassRegistry::initialize();

	/* These timers persist throughout Engine runtime and
		keep track of elapsed times in nanoseconds. */
	Profiler::add_clock(MAIN_THREAD_CLOCK_NAME);
	Profiler::add_clock(ENGINE_LOOP_THREAD_CLOCK_NAME);

	/* Frame skipping clocks*/
	Profiler::add_clock(INPUT_FRAME_SKIP_CLOCK_NAME);
	Profiler::add_clock(UPDATE_FRAME_SKIP_CLOCK_NAME);
	Profiler::add_clock(OUTPUT_FRAME_SKIP_CLOCK_NAME);

	Profiler::add_clock(LAG_CLOCK_NAME);
	Profiler::add_clock(TOTAL_LOOP_FRAME_TIME_CLOCK_NAME);

	do
	{
		logger.write("Initializing Omnific Engine...");
		this->initialize();

		if (this->state != State::FINALIZING)
		{
			std::shared_ptr<Clock> input_frame_skip_clock = Profiler::get_clock(INPUT_FRAME_SKIP_CLOCK_NAME);

			std::shared_ptr<Clock> main_thread_clock = Profiler::get_clock(MAIN_THREAD_CLOCK_NAME);
			uint8_t logical_core_count = Platform::get_logical_core_count();
			Configuration* configuration = Configuration::get_instance();

			if (!configuration->performance_settings.enable_multithreading ||
				logical_core_count < 2 || 
				Platform::get_platform_name() == "Emscripten")
			{
				logger.write("Engine single threading mode enabled.");
#ifdef _WEB_PLATFORM
				emscripten_set_main_loop_arg(run_loop, this, 0, 1);
#else
				/* Single threaded mode. */
				while (this->state == State::INITIALIZING || 
						this->state == State::RUNNING) 
				{
					main_thread_clock->set_start();
					this->detect_input();
					this->run_loop(); 
					main_thread_clock->set_end();
					this->sleep_for(
						configuration->get_max_target_fps(), 
						main_thread_clock);
				}
#endif
			}
			else 
			{
				/* Multithreaded mode. */
				ThreadPool::initialize();
				std::thread engine_loop_thread = std::thread(&Engine::run_loop_on_thread, this);

				while (this->state == State::INITIALIZING || 
					this->state == State::RUNNING) 
				{
					main_thread_clock->set_start();
					this->detect_input();
					main_thread_clock->set_end();
					this->sleep_for(
						configuration->performance_settings.target_input_fps, 
						main_thread_clock);
				}

				engine_loop_thread.join();
				logger.write("Finalizing Engine loops...");
				this->finalize();
			}
		}
	} while (this->state == State::RESTARTING);
}

void Omnific::Engine::initialize()
{
	Logger& logger = Platform::get_logger();

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

	logger.write("Retrieved Logical Core Count: " + std::to_string(Platform::get_logical_core_count()));
	logger.write("Retrieved L1 Cache Line Size: " + std::to_string(Platform::get_l1_cache_line_size()) + " B");
	logger.write("Retrieved OS Name: \"" + Platform::get_platform_name() + "\"");
	logger.write("Retrieved System RAM: " + std::to_string(Platform::get_system_ram()) + " MB");

	std::string app_data_directory = DEFAULT_APP_DATA_DIRECTORY;

#ifdef _DEBUG
	while (true)
	{
		Platform::get_window().hide();
		std::cout << "\n\nChoose data project to load:" << std::endl;
		std::cout << "0. Custom" << std::endl;
		std::cout << "1. Demos" << std::endl;
		std::cout << "2. Editor" << std::endl;
		std::cout << "\n-> #";

		std::string input_string;
		std::cin >> input_string;
		Platform::get_window().show();

		if (input_string == "0")
		{
			break;
		}
		else if (input_string == "1")
		{
			app_data_directory = DEFAULT_DEBUG_DEMO_DATA_DIRECTORY;
			break;
		}
		else if (input_string == "2")
		{
			app_data_directory = DEFAULT_DEBUG_STUDIO_DATA_DIRECTORY;
			break;
		}
		else
		{
			std::cout << "Error: Option \"" + input_string + "\" not listed." << std::endl;
		}
	}
#endif
	FileAccess& file_access = Platform::get_file_access();
	this->app_data_directories.push_back(app_data_directory);
	file_access.set_app_data_directories(app_data_directories);

	std::string boot_filepath = file_access.find_path_among_app_data_directories(BOOT_FILE_NAME);

	if (file_access.exists(boot_filepath))
		Configuration::load_from_file(boot_filepath);

#ifdef DEBUG_CONSOLE_ENABLED
	std::cout << "\n\nPress '`' in-application to write to command line via console.";
	std::cout << "\n\n";
#endif

	if (Configuration::get_instance()->is_loaded)
	{
		Window& window = Platform::get_window();
		window.resize(Configuration::get_instance()->window_settings.width, Configuration::get_instance()->window_settings.height);
		window.change_title(Configuration::get_instance()->metadata.title.c_str());
		logger.write("Loaded application project \"" + Configuration::get_instance()->metadata.title + "\" at: " + app_data_directory);
	}
	else
	{
		Platform::show_error_box(
			"Could not load game data",
			"The game data is either missing or corrupted. Reinstall and try again");
		logger.write("Shutting down Engine due to error in loading Configuration.");
		this->state = State::FINALIZING;
	}

}

void Omnific::Engine::detect_input()
{
	std::shared_ptr<Clock> input_frame_skip_clock = Profiler::get_clock(INPUT_FRAME_SKIP_CLOCK_NAME);
	input_frame_skip_clock->set_end();

	if (input_frame_skip_clock->get_delta() >= 
		MS_IN_S / Configuration::get_instance()->performance_settings.target_input_fps)
	{
		input_frame_skip_clock->set_start();
		Input& input = Platform::get_input();
		input.detect_game_controllers();
		input.poll_input_events();
		if (input.has_requested_shutdown())
			this->state = State::FINALIZING;
		if (input.has_requested_restart())
			this->state = State::RESTARTING;
	}
}

void Omnific::Engine::run_loop()
{
	std::shared_ptr<Clock> total_loop_frame_time_clock = Profiler::get_clock(TOTAL_LOOP_FRAME_TIME_CLOCK_NAME);
	total_loop_frame_time_clock->set_start();

	Configuration* configuration = Configuration::get_instance();

	if (this->state == State::INITIALIZING)
	{
		Platform::get_logger().write("Engine loops currently running...");

		for (auto system : this->systems)
			system.second->initialize();

		FileAccess& file_access = Platform::get_file_access();
		std::string entry_scene_filepath = configuration->metadata.entry_scene_filepath;

		std::string app_data_entry_scene_filepath = file_access.find_path_among_app_data_directories(entry_scene_filepath);
		if (app_data_entry_scene_filepath != "")
			SceneStorage::load_scene(std::shared_ptr<Scene>(new Scene(entry_scene_filepath)));

		this->state = State::RUNNING;
	}

	std::shared_ptr<Scene> active_scene = SceneStorage::get_active_scene();
	std::shared_ptr<Clock> lag_clock = Profiler::get_clock(LAG_CLOCK_NAME);

	std::shared_ptr<Clock> update_frame_skip_clock = Profiler::get_clock(UPDATE_FRAME_SKIP_CLOCK_NAME);
	std::shared_ptr<Clock> output_frame_skip_clock = Profiler::get_clock(OUTPUT_FRAME_SKIP_CLOCK_NAME);
	update_frame_skip_clock->set_end();
	output_frame_skip_clock->set_end();

	if (update_frame_skip_clock->get_delta() >= 
		MS_IN_S / configuration->performance_settings.target_update_fps)
	{
		update_frame_skip_clock->set_start();

		if (Platform::get_input().get_has_detected_input_changes())
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

		uint32_t target_compute_frame_time = Configuration::get_instance()->performance_settings.compute_frame_time;

		while (Profiler::get_lag_count() >= target_compute_frame_time &&
			this->state == State::RUNNING)
		{
			for (auto system : this->systems)
				system.second->on_compute(active_scene);
			Profiler::decrement_lag_count(target_compute_frame_time);
		}

		for (auto system : this->systems)
			system.second->on_late(active_scene);

		for (auto system : this->systems)
			system.second->on_finish(active_scene);

		for (auto it : active_scene->get_scene_layers())
			it.second->clear_finish_entity_queue();
	}

	if ( output_frame_skip_clock->get_delta() >= 
		MS_IN_S / configuration->performance_settings.target_output_fps)
	{
		output_frame_skip_clock->set_start();
		for (auto system : this->systems)
			system.second->on_output(active_scene);
	}

	lag_clock->set_end();
	lag_clock->set_start();

	EventBus::clear();
	Profiler::increment_lag_count(lag_clock->get_delta());

	if (this->state == State::RESTARTING || 
		this->state == State::FINALIZING) 
	{
        for (auto system : this->systems)
			system.second->finalize();

#ifdef _WEB_PLATFORM
        emscripten_cancel_main_loop();
#else
        return;
#endif
    }

	total_loop_frame_time_clock->set_end();
}

void Omnific::Engine::run_loop_on_thread()
{
	std::shared_ptr<Clock> engine_loop_thread_clock = Profiler::get_clock(ENGINE_LOOP_THREAD_CLOCK_NAME);
	while (this->state == State::INITIALIZING || 
		this->state == State::RUNNING)
	{
		engine_loop_thread_clock->set_start();
		this->run_loop();
		engine_loop_thread_clock->set_end();
		this->sleep_for(
			Configuration::get_instance()->get_max_target_fps(), 
			engine_loop_thread_clock);
	}
}

void Omnific::Engine::sleep_for(uint32_t target_fps, std::shared_ptr<Clock> run_timer)
{
	float target_frame_time = MS_IN_S / target_fps;
	float run_time = run_timer->get_delta();
	Platform::sleep_thread_for(target_frame_time - run_time);
}

void Omnific::Engine::finalize()
{
	SceneStorage::clear_scenes();
	ThreadPool::finalize();
}