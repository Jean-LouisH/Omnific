#pragma once

#include "Scene/Scene.hpp"
#include "../Utilities/String.hpp"

namespace Lilliputian
{
	class SceneLoader
	{
	private:
		String assetsDirectory;
	public:
		SceneLoader(String assetsDirectory);
		Scene loadScene(const char* filepath);
	};
}