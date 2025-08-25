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

#ifdef _WEB_PLATFORM
#include <emscripten.h>
#endif

Omnific::Engine::Engine(int argc, char* argv[])
{
	Platform::initialize(argc, argv);
}

void Omnific::Engine::add_app_data_paths(std::vector<std::string> app_data_paths)
{
	Platform::get_file_access().add_app_data_paths(app_data_paths);
}

void Omnific::Engine::run()
{
	Logger& logger = Platform::get_logger();

	/* These timers persist throughout Engine runtime */
	Profiler::add_clock(MAIN_THREAD_CLOCK_NAME, {"thread"});

	Profiler::add_clock(UPDATE_LOOP_FRAME_SKIP_CLOCK_NAME, {"loop_frame_skip"});
	Profiler::add_clock(OUTPUT_LOOP_FRAME_SKIP_CLOCK_NAME, {"loop_frame_skip"});

	Profiler::add_clock(UPDATE_LAG_CLOCK_NAME, {"lag"});

	Profiler::add_clock(UPDATE_LOOP_FRAME_TIME_CLOCK_NAME, {"loop_frame_time"});
	Profiler::add_clock(OUTPUT_LOOP_FRAME_TIME_CLOCK_NAME, {"loop_frame_time"});
	Profiler::add_clock(TOTAL_LOOP_FRAME_TIME_CLOCK_NAME, {"loop_frame_time"});

	Profiler::add_clock(TOTAL_ON_INPUT_FRAME_TIME_CLOCK_NAME, {"total", "on_input_frame_time"});
	Profiler::add_clock(TOTAL_ON_ENTITY_START_FRAME_TIME_CLOCK_NAME, {"total", "on_entity_start_frame_time"});
	Profiler::add_clock(TOTAL_ON_EARLY_UPDATE_FRAME_TIME_CLOCK_NAME, {"total", "on_early_update_frame_time"});
	Profiler::add_clock(TOTAL_ON_UPDATE_FRAME_TIME_CLOCK_NAME, {"total", "on_update_frame_time"});
	Profiler::add_clock(TOTAL_ON_FIXED_UPDATE_FRAME_TIME_CLOCK_NAME, {"total", "on_fixed_update_frame_time"});
	Profiler::add_clock(TOTAL_ON_LATE_UPDATE_FRAME_TIME_CLOCK_NAME, {"total", "on_late_update_frame_time"});
	Profiler::add_clock(TOTAL_ON_ENTITY_FINISH_FRAME_TIME_CLOCK_NAME, {"total", "on_entity_finish_frame_time"});
	Profiler::add_clock(TOTAL_ON_OUTPUT_FRAME_TIME_CLOCK_NAME, {"total", "on_output_frame_time"});

	Profiler::add_clock("debug_1", {"debug"});
	Profiler::add_clock("debug_2", {"debug"});
	Profiler::add_clock("debug_3", {"debug"});
	
	do
	{
		this->initialize();

		if (this->state != State::FINALIZING)
		{
#ifdef _WEB_PLATFORM
			emscripten_set_main_loop_arg(run_loop_on_callback, this, 0, 1);
#else
			std::shared_ptr<Clock> main_thread_clock = Profiler::get_clock(MAIN_THREAD_CLOCK_NAME);
			Configuration* configuration = Configuration::get_instance();

			while (this->state == State::INITIALIZING || 
					this->state == State::RUNNING) 
			{
				main_thread_clock->set_start();
				this->run_loop(); 
				main_thread_clock->set_end();
				this->sleep_for(
					configuration->get_max_target_fps(), 
					main_thread_clock);
			}
#endif
		}

		logger.write("Finalizing Engine...");
		this->finalize();
		
	} while (this->state == State::RESTARTING);
}

void Omnific::Engine::initialize()
{
	Logger& logger = Platform::get_logger();
	logger.write("Initializing Omnific Engine...");
	logger.write("Querying Platform...");
	logger.write("Retrieved Logical Core Count: " + std::to_string(Platform::get_logical_core_count()));
	logger.write("Retrieved L1 Cache Line Size: " + std::to_string(Platform::get_l1_cache_line_size()) + " B");
	logger.write("Retrieved OS Name: \"" + Platform::get_platform_name() + "\"");
	logger.write("Retrieved System RAM: " + std::to_string(Platform::get_system_ram()) + " MB");

	if (this->state != State::RESTARTING)
		this->state = State::INITIALIZING;

	ThreadPool::initialize();

	FileAccess& file_access = Platform::get_file_access();
	file_access.add_app_data_paths({"./", DEFAULT_APP_DATA_PATH});
	std::string boot_filepath = file_access.find_path(BOOT_FILE_NAME);

	if (boot_filepath != "")
		Configuration::load_from_file(boot_filepath);

	ClassRegistry::initialize();
	logger.write("Loading Systems from ClassRegistry...");

	for (auto it : ClassRegistry::query_all<System>())
	{
		std::shared_ptr<System> system = std::dynamic_pointer_cast<System>(it.second);

		this->systems.emplace(
			it.first,
			std::dynamic_pointer_cast<System>(std::shared_ptr<Registerable>(system->instance())));
	}

#ifdef DEBUG_CONSOLE_ENABLED
	std::cout << "\n\nPress '`' in-application to write to command line via console.";
	std::cout << "\n\n";
#endif

	if (Configuration::get_instance()->is_loaded)
	{
		Window& window = Platform::get_window();
		window.resize(Configuration::get_instance()->window_settings.width, Configuration::get_instance()->window_settings.height);
		window.change_title(Configuration::get_instance()->metadata.title.c_str());
		logger.write("Loaded application project \"" + Configuration::get_instance()->metadata.title + "\"");
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

void Omnific::Engine::run_loop()
{
	std::shared_ptr<Clock> total_loop_frame_time_clock = Profiler::get_clock(TOTAL_LOOP_FRAME_TIME_CLOCK_NAME);
	total_loop_frame_time_clock->set_start();

	Configuration* configuration = Configuration::get_instance();
	Inputs& inputs = Platform::get_inputs();

	if (this->state == State::INITIALIZING)
	{
		Platform::get_logger().write("Engine currently running...");

		for (auto system : this->systems)
			system.second->initialize();

		FileAccess& file_access = Platform::get_file_access();
		std::string entry_scene_filepath = configuration->metadata.entry_scene_filepath;

		std::string app_data_entry_scene_filepath = file_access.find_path(entry_scene_filepath);
		if (app_data_entry_scene_filepath != "")
		{
			SceneStorage::load_scene(std::shared_ptr<Scene>(new Scene(entry_scene_filepath)));
		}

		this->state = State::RUNNING;
	}

	std::shared_ptr<Scene> active_scene = SceneStorage::get_active_scene();
	std::string name = active_scene->get_name();
	std::shared_ptr<Clock> update_lag_clock = Profiler::get_clock(UPDATE_LAG_CLOCK_NAME);

	std::shared_ptr<Clock> update_loop_frame_skip_clock = Profiler::get_clock(UPDATE_LOOP_FRAME_SKIP_CLOCK_NAME);
	std::shared_ptr<Clock> output_loop_frame_skip_clock = Profiler::get_clock(OUTPUT_LOOP_FRAME_SKIP_CLOCK_NAME);

	std::shared_ptr<Clock> update_loop_frame_time_clock = Profiler::get_clock(UPDATE_LOOP_FRAME_TIME_CLOCK_NAME);
	std::shared_ptr<Clock> output_loop_frame_time_clock = Profiler::get_clock(OUTPUT_LOOP_FRAME_TIME_CLOCK_NAME);

	std::shared_ptr<Clock> total_on_input_frame_time_clock = Profiler::get_clock(TOTAL_ON_INPUT_FRAME_TIME_CLOCK_NAME);
	std::shared_ptr<Clock> total_on_entity_start_frame_time_clock = Profiler::get_clock(TOTAL_ON_ENTITY_START_FRAME_TIME_CLOCK_NAME);
	std::shared_ptr<Clock> total_on_early_update_frame_time_clock = Profiler::get_clock(TOTAL_ON_EARLY_UPDATE_FRAME_TIME_CLOCK_NAME);
	std::shared_ptr<Clock> total_on_update_frame_time_clock = Profiler::get_clock(TOTAL_ON_UPDATE_FRAME_TIME_CLOCK_NAME);
	std::shared_ptr<Clock> total_on_fixed_update_frame_time_clock = Profiler::get_clock(TOTAL_ON_FIXED_UPDATE_FRAME_TIME_CLOCK_NAME);
	std::shared_ptr<Clock> total_on_late_update_frame_time_clock = Profiler::get_clock(TOTAL_ON_LATE_UPDATE_FRAME_TIME_CLOCK_NAME);
	std::shared_ptr<Clock> total_on_entity_finish_frame_time_clock = Profiler::get_clock(TOTAL_ON_ENTITY_FINISH_FRAME_TIME_CLOCK_NAME);
	std::shared_ptr<Clock> total_on_output_frame_time_clock = Profiler::get_clock(TOTAL_ON_OUTPUT_FRAME_TIME_CLOCK_NAME);

	update_loop_frame_skip_clock->set_end();

	if (update_loop_frame_skip_clock->get_delta() >= 
		MS_IN_S / configuration->performance_settings.target_update_fps)
	{
		update_loop_frame_time_clock->set_start();
		update_loop_frame_skip_clock->set_start();

		inputs.detect_game_controllers();
		inputs.poll_input_events();

		if (inputs.has_requested_shutdown())
			this->state = State::FINALIZING;
		if (inputs.has_requested_restart())
			this->state = State::RESTARTING;

		total_on_input_frame_time_clock->set_start();
		if (Platform::get_inputs().get_has_detected_input_changes())
			for (auto system : this->systems)
				system.second->on_input(active_scene);
		total_on_input_frame_time_clock->set_end();

		total_on_entity_start_frame_time_clock->set_start();
		for (auto system : this->systems)
			system.second->on_entity_start(active_scene);
		total_on_entity_start_frame_time_clock->set_end();

		active_scene->clear_start_entity_queue();

		total_on_early_update_frame_time_clock->set_start();
		for (auto system : this->systems)
			system.second->on_early_update(active_scene);
		total_on_early_update_frame_time_clock->set_end();

		total_on_update_frame_time_clock->set_start();
		for (auto system : this->systems)
			system.second->on_update(active_scene);
		total_on_update_frame_time_clock->set_end();

		/* This calls the compute based Systems repeatedly until the accumulated
			lag milliseconds are depleted. This ensures compute operations
			are accurate to real-time, even when frames drop. */

		uint32_t target_fixed_frame_time = Configuration::get_instance()->performance_settings.fixed_frame_time;

		output_loop_frame_skip_clock->set_end();
		total_on_fixed_update_frame_time_clock->set_start();
		while (Profiler::get_lag_count() >= target_fixed_frame_time &&
			this->state == State::RUNNING)
		{
			for (auto system : this->systems)
				system.second->on_fixed_update(active_scene);
			Profiler::decrement_lag_count(target_fixed_frame_time);
		}
		total_on_fixed_update_frame_time_clock->set_end();

		total_on_late_update_frame_time_clock->set_start();
		for (auto system : this->systems)
			system.second->on_late_update(active_scene);
		total_on_late_update_frame_time_clock->set_end();

		total_on_entity_finish_frame_time_clock->set_start();
		for (auto system : this->systems)
			system.second->on_entity_finish(active_scene);
		total_on_entity_finish_frame_time_clock->set_end();

		active_scene->clear_finish_entity_queue();

		update_loop_frame_time_clock->set_end();
	}

	if ( output_loop_frame_skip_clock->get_delta() >= 
		MS_IN_S / configuration->performance_settings.target_output_fps)
	{
		total_on_output_frame_time_clock->set_start();
		output_loop_frame_time_clock->set_start();
		output_loop_frame_skip_clock->set_start();
		for (auto system : this->systems)
			system.second->on_output(active_scene);
		output_loop_frame_time_clock->set_end();
		total_on_output_frame_time_clock->set_end();
	}

	update_lag_clock->set_end();
	update_lag_clock->set_start();

	EventBus::clear_instant_events();
	Profiler::increment_lag_count(update_lag_clock->get_delta());

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
	active_scene->update_debug_statistics();
}

 void Omnific::Engine::run_loop_on_callback(void* arg)
 {
	Engine* engine = (Engine*)arg;
	engine->run_loop();
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
	ClassRegistry::finalize();
	this->systems.clear();
}