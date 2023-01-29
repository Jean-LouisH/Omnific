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

#include "core/component.hpp"
#include <core/assets/image.hpp>
#include <core/assets/shader.hpp>
#include <memory>
#include <glm/glm.hpp>
#include <engine_api.hpp>

namespace Omnia
{

	class OMNIA_ENGINE_API RenderableComponent : public Component
	{
	public:
		enum class CullMode
		{
			NONE,
			BACK,
			FRONT,
			FRONT_AND_BACK
		};

		void setDimensions(float width, float height);
		void setDimensions(float width, float height, float depth);
		void addShader(std::shared_ptr<Shader> shader);
		void setAlpha(uint8_t value);
		void setToNoFaceCulling();
		void setToFrontFaceCulling();
		void setToBackFaceCulling();
		void setToFrontAndBackFaceCulling();
		uint8_t getAlpha();
		float getAlphaInPercentage();
		bool isNoFaceCulling();
		bool isFrontFaceCulling();
		bool isBackFaceCulling();
		bool isFrontAndBackFaceCulling();
		void hide();
		void show();
		CullMode getCullMode();
		std::shared_ptr<Image> getImage();
		std::vector<std::shared_ptr<Shader>> getShaders();
		glm::vec3 getDimensions();
		bool isRenderable() override;
	protected:
		glm::vec3 dimensions;
		uint8_t alpha = 255;
		CullMode cullMode = CullMode::NONE;
		std::shared_ptr<Image> image;
		std::vector<std::shared_ptr<Shader>> shaders;
	private:
	};
}