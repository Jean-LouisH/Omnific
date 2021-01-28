#pragma once

#include "scene/scene.hpp"
#include "../utilities/string.hpp"

namespace Lilliputian
{
	class SceneSerializer
	{
	public:
		SceneSerializer(String assetsDirectory);
		Scene loadFromFile(const char* filepath);
	private:
		String assetsDirectory;
	};
}