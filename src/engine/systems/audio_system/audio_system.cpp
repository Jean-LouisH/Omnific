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

#include "audio_system.hpp"
#include <foundations/singletons/platform/platform.hpp>
#include <foundations/singletons/profiler.hpp>
#include "scene/components/audio_listener.hpp"
#include <scene/components/audio_source.hpp>
#include <foundations/singletons/event_bus.hpp>

Omnific::AudioSystem::AudioSystem()
{
	this->type = TYPE_STRING;
	this->openal_backend = std::shared_ptr<OpenALAudioBackend>(new OpenALAudioBackend());
}

Omnific::AudioSystem::~AudioSystem()
{
	this->finalize();
}

void Omnific::AudioSystem::initialize()
{
	this->is_initialized = true;
	this->alc_device = alcOpenDevice(NULL);
	this->alc_context = alcCreateContext(this->alc_device, NULL);
	alcMakeContextCurrent(this->alc_context);

	this->has_eax2_0_extension = alIsExtensionPresent("EAX2.0");
	alGetError();

	this->openal_backend->initialize();
	Platform::get_logger().write("Initialized Audio System.");
}

void Omnific::AudioSystem::on_output(std::shared_ptr<Scene> scene)
{
	std::map<SceneLayerID, std::shared_ptr<SceneLayer>>& scene_layers = scene->get_scene_layers();

	for (auto it = scene_layers.begin(); it != scene_layers.end(); it++)
	{
		std::shared_ptr<SceneLayer> scene_layer = it->second;
		std::vector<std::shared_ptr<AudioListener>> audio_listeners = scene_layer->get_components_by_type<AudioListener>();

		if (audio_listeners.size() > 0)
		{
			std::shared_ptr<AudioListener> audio_listener = audio_listeners.at(audio_listeners.size() - 1); //Get the last AudioListener.
			std::shared_ptr<Entity> listener_entity = scene_layer->get_entity(audio_listener->get_entity_id());
			std::shared_ptr<Transform> listener_global_transform = scene_layer->calculate_global_transform(listener_entity->get_id());
			std::shared_ptr<PhysicsBody> listener_physics_body = scene_layer->get_component_by_type<PhysicsBody>(listener_entity->get_id());

			std::vector<std::shared_ptr<AudioSource>> audio_sources = scene_layer->get_components_by_type<AudioSource>();
			std::unordered_map<ComponentID, std::shared_ptr<AudioSource>> audio_source_dictionary = scene_layer->get_components_by_type_in_dictionary<AudioSource>();

			if (scene_layer->is_2d)
				listener_global_transform->flatten_to_2d();

			std::shared_ptr<OpenALListener> listener = this->openal_backend->get_listener(audio_listener);
			listener->set_gain(audio_listener->get_volume());
			listener->set_position(listener_global_transform->translation);
			listener->set_orientation(listener_global_transform->get_up_vector());
		
			if (listener_physics_body != nullptr)
				listener->set_velocity(listener_physics_body->linear_velocity);

			for (std::shared_ptr<AudioSource> audio_source: audio_sources)
			{
				std::shared_ptr<Entity> source_entity = scene_layer->get_entity(audio_source->get_entity_id());
				std::shared_ptr<PhysicsBody> source_physics_body = scene_layer->get_component_by_type<PhysicsBody>(audio_source->get_entity_id());
				std::shared_ptr<Transform> source_global_transform = scene_layer->calculate_global_transform(source_entity->get_id());
				if (scene_layer->is_2d)
					source_global_transform->flatten_to_2d();
				std::shared_ptr<OpenALSource> source = this->openal_backend->get_source(audio_source);
				std::shared_ptr<OpenALBuffer> buffer = this->openal_backend->get_buffer(audio_source->get_active_audio());
				source->attach_buffer(buffer->get_id());
				source->set_gain(audio_source->get_volume());
				source->set_position(source_global_transform->translation);
				source->set_direction(source_global_transform->rotation);
				if (source_physics_body != nullptr)
					source->set_velocity(source_physics_body->linear_velocity);
				audio_source->playback_time = source->get_playback_position();
			}

			for (Event jumping_event : EventBus::query_events("jumping_audio_source"))
			{
				std::shared_ptr<AudioSource> audio_source = audio_source_dictionary[(ComponentID)jumping_event.get_parameters().numbers["id"]];
				float time_point = jumping_event.get_parameters().numbers["time_point"];
				audio_source->playback_time = time_point;
				this->openal_backend->get_source(audio_source)->set_playback_position_by_seconds(time_point);
				EventBus::remove_continuous_event(jumping_event.get_name(), audio_source->get_active_audio()->get_name());
			}

			for (Event reseting_event : EventBus::query_events("reseting_audio_source"))
			{
				std::shared_ptr<AudioSource> audio_source = audio_source_dictionary[(ComponentID)reseting_event.get_parameters().numbers["id"]];
				this->openal_backend->get_source(audio_source)->rewind();
				EventBus::remove_continuous_event(reseting_event.get_name(), audio_source->get_active_audio()->get_name());
			}

			for (Event resuming_event : EventBus::query_events("resuming_audio_source"))
			{
				std::shared_ptr<AudioSource> audio_source = audio_source_dictionary[(ComponentID)resuming_event.get_parameters().numbers["id"]];
				this->openal_backend->get_source(audio_source)->play();
				EventBus::remove_continuous_event(resuming_event.get_name(), audio_source->get_active_audio()->get_name());
			}

			for (Event pausing_event : EventBus::query_events("pausing_audio_source"))
			{
				std::shared_ptr<AudioSource> audio_source = audio_source_dictionary[(ComponentID)pausing_event.get_parameters().numbers["id"]];
				this->openal_backend->get_source(audio_source)->pause();
				EventBus::remove_continuous_event(pausing_event.get_name(), audio_source->get_active_audio()->get_name());
			}

			for (Event stopping_event : EventBus::query_events("stopping_audio_source"))
			{
				std::shared_ptr<AudioSource> audio_source = audio_source_dictionary[(ComponentID)stopping_event.get_parameters().numbers["id"]];
				this->openal_backend->get_source(audio_source)->stop();
				EventBus::remove_continuous_event(stopping_event.get_name(), audio_source->get_active_audio()->get_name());
			}

			for (Event playing_event : EventBus::query_events("playing_audio_source"))
			{
				std::shared_ptr<AudioSource> audio_source = audio_source_dictionary[(ComponentID)playing_event.get_parameters().numbers["id"]];
				std::shared_ptr<OpenALSource> source = this->openal_backend->get_source(audio_source);
				source->stop();
				source->set_is_looping(audio_source->is_looping);
				source->play();
				EventBus::remove_continuous_event(playing_event.get_name(), audio_source->get_active_audio()->get_name());
			}
		}
	}
	this->openal_backend->collect_garbage();
}

void Omnific::AudioSystem::finalize()
{
	if (this->is_initialized)
	{
		alcMakeContextCurrent(NULL);
		alcDestroyContext(this->alc_context);
		alcCloseDevice(this->alc_device);
	}

	this->is_initialized = false;
}