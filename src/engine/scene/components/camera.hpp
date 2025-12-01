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

#pragma once

#include "foundations/aliases.hpp"
#include <glm.hpp>
#include "foundations/constants.hpp"
#include "scene/components/component.hpp"
#include "foundations/resources/shader.hpp"


namespace Omnific
{
	class OMNIFIC_ENGINE_API Camera : public Component
	{
		enum class ControllerState
		{
			NONE,
			FLYBY,
			FOLLOW,
			FOLLOW_GROUP,
			FOLLOW_BY_ZOOMING,
			LOOK_AT,
			LOOK_AT_GROUP,
			CINEMATIC
		};

		friend class CameraSystem;
		friend class RenderingSystem;
	public:
		static constexpr const char* TYPE_STRING = "Camera";
		Camera()
		{
			this->type = TYPE_STRING;
			this->set_viewport_height(480);
		};

		virtual Registerable* instance() override
		{
			Camera* clone = new Camera(*this);
			clone->id = UIDGenerator::get_new_uid();
			return clone;
		}
		virtual void deserialize(YAML::Node yaml_node);
		void reset_aspect();
		void toggle_wireframe_mode();
		glm::vec2 get_viewport();
		bool get_is_streaming();
		bool get_is_wireframe_mode();
		void set_viewport_width(uint32_t width);
		void set_viewport_height(uint32_t height);
		void set_limits(float left, float bottom, float top, float right);
		void set_keep_aspect(bool value);
		void set_is_streaming(bool value);
		void set_wireframe_mode(bool value);
		void set_controller_state_by_string(std::string state);
		glm::mat4 get_view_to_projection_matrix();

		glm::vec3 clear_colour;
		ControllerState controller_state;
		float flyby_translation_speed = 3.0;
		float flyby_rotation_speed = 60.0;
		bool enable_flyby_mode_on_default_input = false;
	private:
		const float default_aspect = 1920.0 / 1080.0;
		float aspect = default_aspect;
		float field_of_view = 45.0;
		float near_plane = 0.1;
		float far_plane = 1000.0;
		glm::vec2 min_limit;
		glm::vec2 max_limit;
		glm::vec2 viewport;
		glm::vec3 linear_velocity;
		glm::vec3 angular_velocity;

		bool keep_aspect = true;
		bool is_streaming = true;
		bool is_wire_frame = false;
		std::string follow_target_entity;
		std::string viewport_target_entity;
		std::shared_ptr<Shader> shader;

		struct FollowMode
		{
			float target_max_linear_velocity;
			float target_max_angular_velocity;
			float linear_deceleration = 400;
		} follow_mode;

		struct FlybyMode
		{
			float target_max_linear_velocity;
			float target_max_angular_velocity;
			float linear_deceleration = 400;
		} flyby_mode;
	};
}