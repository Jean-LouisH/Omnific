#pragma once

#include "../Utilities/Image.hpp"

namespace Lilliputian
{
	class RenderAPI
	{
	private:
	public:
		Image captureScreenshot();
		void captureScreenshotToFile(const char* filename);
	};
}