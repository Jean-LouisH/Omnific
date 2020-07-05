#pragma once

#include "../Utilities/Aliases.hpp"
#include "../Utilities/Collections/Vector.hpp"
#include "Texture.hpp"

namespace Lilliputian
{
	class Sprite2D
	{
	private:
		enum FlipStates
		{
			FLIP_HORIZONTAL = 1 << 0,
			FLIP_VERTICAL = 1 << 1
		};

		EntityID entityID;
		Vector<Texture> textureFrames;
		uint8_t flip;
	public:
		uint8_t frameIndex;
		uint8_t alpha;

		Sprite2D(EntityID entityID);
		void addTextureFrame(Texture texture);
		void flipHorizontally();
		void flipVertically();
		bool getVerticalFlip();
		bool getHorizontalFlip();
		Vector<Texture> getTextureFrames();
		EntityID getEntityID();
	};
}