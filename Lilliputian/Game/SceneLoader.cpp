#include "SceneLoader.hpp"
#include <yaml-cpp/yaml.h>
#include "../Utilities/String.hpp"

Lilliputian::SceneLoader::SceneLoader(String assetsDirectory)
{
	this->assetsDirectory = assetsDirectory;
}

Lilliputian::Scene Lilliputian::SceneLoader::loadScene(const char* filepath)
{
	Scene scene;
	String fullFilepath = this->assetsDirectory + filepath;

	try
	{
		YAML::Node node = YAML::LoadFile(fullFilepath.c_str());
	}
	catch (int e)
	{

	}

	return scene;
}