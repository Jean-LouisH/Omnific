#pragma once

#include "Scene/Scene.hpp"
#include "../Utilities/String.hpp"

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