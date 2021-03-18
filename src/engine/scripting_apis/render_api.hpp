#pragma once

#include "game/scene/assets/image.hpp"

namespace Lilliputian
{
	class RenderAPI
	{
	public:
		Image captureScreenshot();
		void captureScreenshotToFile(const char* filename);
	private:
	};
}