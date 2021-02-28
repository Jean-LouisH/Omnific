#pragma once

#include "scene/scene_forest.hpp"
#include "utilities/string.hpp"

namespace Lilliputian
{
	class SceneSerializer
	{
	public:
		SceneSerializer(String assetsDirectory);
		SceneForest loadFromFile(String filepath);
	private:
		String dataDirectory;
	};
}