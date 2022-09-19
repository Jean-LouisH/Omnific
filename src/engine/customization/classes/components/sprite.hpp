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

#include "utilities/aliases.hpp"
#include <vector>
#include <unordered_map>
#include "utilities/constants.hpp"
#include <string>
#include "customization/classes/assets/image.hpp"
#include "renderable_component.hpp"
#include <glm/glm.hpp>
#include <memory>

namespace Omnia
{
	class OMNIA_ENGINE_API Sprite : public RenderableComponent
	{
	public:
		Sprite()
		{
			this->type = TYPE_STRING;
		};
		static constexpr const char* TYPE_STRING = "Sprite";
		std::string currentFrameSequenceName = "";

		bool isXBillboarded = false;
		bool isYBillboarded = false;

		virtual Registerable* instance() override
		{
			Sprite* clone = new Sprite(*this);
			clone->id = UIDGenerator::getNewUID();
			return clone;
		}
		virtual void deserialize(YAML::Node yamlNode);
		void addImage(std::shared_ptr<Image> image);
		void addEmptyFrameSequence(std::string frameSequenceName);
		void addFrameSequence(std::string frameSequenceName, std::vector<std::shared_ptr<Image>> frameSequence);
		void addImageToFrameSequence(std::string frameSequenceName, std::shared_ptr<Image> frame);
		void clearFrameSequences();

		void setAnimationSpeed(float value_fps);
		float getAnimationSpeed();

		void update(float delta_s);

		void play(std::string frameSequenceName);
		void play();
		void pause();
		void stop();

		void setBackwards();
		void setForwards();

		void flipVertically();
		void flipHorizontally();

		std::shared_ptr<Image> getCurrentFrame();
		std::vector<std::string> getFrameSequenceNames();
		std::vector<std::shared_ptr<Image>> getFrameSequenceByName(std::string frameSequenceName);
		std::vector<std::shared_ptr<Image>> getCurrentFrameSequence();
	private:
		std::unordered_map<std::string, std::vector<std::shared_ptr<Image>>> frameSequences;
		float animationSpeedInFPS = 12.0;
		float frameTime = 0.0;
		uint16_t currentFrameIndex = 0;
		glm::vec3 billboardOffsetRotation = {0.0, 0.0, 0.0};
		bool isPlaying = false;
		bool isBackwards = false;
		bool isFlippedVertically = false;
		bool isFlippedHorizontally = false;
	};
}