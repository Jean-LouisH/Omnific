#pragma once

#include "../Utilities/Image.hpp"

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