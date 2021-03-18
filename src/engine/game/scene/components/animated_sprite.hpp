#pragma once

#include "utilities/aliases.hpp"
#include "utilities/collections/vector.hpp"
#include "utilities/collections/map.hpp"
#include "utilities/constants.hpp"
#include "utilities/string.hpp"
#include "game/scene/assets/image.hpp"
#include "component.hpp"

namespace Lilliputian
{
	class AnimatedSprite : public Component
	{
		using Frame = Image;
		using FrameIndex = uint16_t;
		using FrameSequence = Vector<Frame>;
	public:
		String currentFrameSequenceName = "";

		void addEmptyFrameSequence(String frameSequenceName);
		void addFrameSequence(String frameSequenceName, FrameSequence frameSequence);
		void addFrameToFrameSequence(String frameSequenceName, Frame frame);
		void clearFrameSequences();

		void setAlpha(uint8_t value);
		uint8_t getAlpha();
		void hide();
		void show();

		void setAnimationSpeed(float value_fps);
		float getAnimationSpeed();

		void update(float delta_s);

		void play(String frameSequenceName);
		void play();
		void pause();
		void stop();

		void setBackwards();
		void setForwards();

		void flipVertically();
		void flipHorizontally();

		Frame getCurrentFrame();
		Vector<String> getFrameSequenceNames();
		FrameSequence getFrameSequenceByName(String frameSequenceName);
		FrameSequence getCurrentFrameSequence();
	private:
		Map<String, FrameSequence> frameSequences;
		uint8_t alpha = 255;
		float animationSpeed_fps = 12.0;
		float frameTime_s = 0.0;
		FrameIndex currentFrameIndex = 0;
		bool isPlaying = false;
		bool isBackwards = false;
		bool isFlippedVertically = false;
		bool isFlippedHorizontally = false;
	};
}