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

#include "openal_audio_backend.hpp"
#include <foundations/singletons/platform/platform.hpp>
#include <string>

void Omnific::OpenALAudioBackend::initialize()
{
// 	Window& window = Platform::get_window();
// #ifdef _WEB_PLATFORM
// 	window.initialize_window_context("webgl");
// #else
// 	window.initialize_window_context("opengl");

// 	if ((!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)))
// 	{
// 		Platform::get_logger().write("GLAD failed to initialize.");
// 	}
// 	else
// #endif
// 	{
// 		this->built_in_shader_program_2d = std::shared_ptr<OpenGLShaderProgram>(new OpenGLShaderProgram(std::shared_ptr<Shader>(new Shader(
// 			this->get_default_2d_vertex_input(),
// 			this->get_default_2d_fragment_input(),
// 			false,
// 			false))));

// 		this->built_in_shader_program_3d = std::shared_ptr<OpenGLShaderProgram>(new OpenGLShaderProgram(std::shared_ptr<Shader>(new Shader(
// 			this->get_default_3d_vertex_input(),
// 			this->get_default_3d_fragment_input(),
// 			false,
// 			false))));

// 		glm::vec2 window_size = window.get_window_size();
// 		this->set_viewport(window_size.x, window_size.y);
// 		Platform::get_logger().write((std::string)("Rendering Backend initialized with ") +
// 			"OpenGL " + (char*)glGetString(GL_VERSION));
// 	}
}

std::shared_ptr<Omnific::OpenALBuffer> Omnific::OpenALAudioBackend::get_buffer(std::shared_ptr<Audio> audio)
{
	std::shared_ptr<OpenALBuffer> buffer;

	if (audio != nullptr)
	{
		AssetID asset_id = audio->get_id();

		if (this->buffers.count(asset_id) == 0)
		{
			this->buffers.emplace(asset_id, std::shared_ptr<OpenALBuffer>(new OpenALBuffer(audio)));
			this->missed_frame_counts.emplace(asset_id, 0);
		}
		else
		{
			this->missed_frame_counts.at(asset_id) = 0;
		}

		buffer = this->buffers.at(asset_id);
	}
	else
	{
		buffer = std::shared_ptr<OpenALBuffer>(new OpenALBuffer());
	}

	return buffer;
}

std::shared_ptr<Omnific::OpenALListener> Omnific::OpenALAudioBackend::get_listener(std::shared_ptr<AudioListener> audio_listener)
{
	ComponentID listener_id;
	
	if (audio_listener != nullptr)
		listener_id = audio_listener->get_id();

	if (this->listeners.count(listener_id) == 0)
	{
		this->listeners.emplace(listener_id, std::shared_ptr<OpenALListener>(new OpenALListener(audio_listener)));
		this->missed_frame_counts.emplace(listener_id, 0);
	}
	else
	{
		this->missed_frame_counts.at(listener_id) = 0;
	}

	return this->listeners.at(listener_id);
}

std::shared_ptr<Omnific::OpenALSource> Omnific::OpenALAudioBackend::get_source(std::shared_ptr<AudioSource> audio_source)
{
	ComponentID source_id;
	
	if (audio_source != nullptr)
		source_id = audio_source->get_id();

	if (this->sources.count(source_id) == 0)
	{
		this->sources.emplace(source_id, std::shared_ptr<OpenALSource>(new OpenALSource(audio_source)));
		this->missed_frame_counts.emplace(source_id, 0);
	}
	else
	{
		this->missed_frame_counts.at(source_id) = 0;
	}

	return this->sources.at(source_id);
}

void Omnific::OpenALAudioBackend::collect_garbage()
{
	std::vector<UID> objects_to_delete;

	for (auto it = this->missed_frame_counts.begin(); it != this->missed_frame_counts.end(); it++)
	{
		if (this->missed_frame_counts.at(it->first) > this->allowable_missed_frames)
		{
			if (this->buffers.count(it->first) > 0)
				this->buffers.erase(it->first);
			if (this->listeners.count(it->first) > 0)
				this->listeners.erase(it->first);
			if (this->sources.count(it->first) > 0)
				this->sources.erase(it->first);

				objects_to_delete.push_back(it->first);
		}
	}

	for (size_t i = 0; i < objects_to_delete.size(); i++)
		this->missed_frame_counts.erase(objects_to_delete.at(i));

	for (auto it = this->missed_frame_counts.begin(); it != this->missed_frame_counts.end(); it++)
		this->missed_frame_counts.at(it->first)++;
}