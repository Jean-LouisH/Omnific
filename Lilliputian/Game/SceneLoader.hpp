#pragma once

#include "Scene/Scene.hpp"
#include "../Utilities/String.hpp"

namespace Lilliputian
{
	class SceneLoader
	{
	public:
		SceneLoader(String assetsDirectory);
		Scene loadScene(const char* filepath);
	private:
		String assetsDirectory;
	};
}