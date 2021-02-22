#include "scene_serializer.hpp"
#include <yaml-cpp/yaml.h>
#include "utilities/string.hpp"
#include "scene/scene.hpp"

Lilliputian::SceneSerializer::SceneSerializer(String assetsDirectory)
{
	this->assetsDirectory = assetsDirectory;
}

Lilliputian::Scene Lilliputian::SceneSerializer::loadFromFile(const char* filepath)
{
	Scene scene;
	const std::string fullFilepath = this->assetsDirectory + filepath;

	try
	{
		YAML::Node yamlNode = YAML::LoadFile(fullFilepath);

		for (YAML::const_iterator it0 = yamlNode.begin(); it0 != yamlNode.end(); ++it0)
		{
			if (it0->first.as<std::string>() == "Subscene2D")
			{
				scene.incrementSceneLayer2D();

				/*for (YAML::const_iterator it1 = it0->second.begin(); it1 != it0->second.end(); ++it1)
				{
					if (it1->first.as<std::string>() == "Entity2D")
					{
						String name = "";
						Transform2D transform2D;

						bool hasParent = false;
						String parentName;

						bool hasCamera2D = false;
						Vector2 cameraViewport_px;

						bool hasSprite2D = false;
						Vector<String> textureFrameFilePaths;

						bool hasRigidBody2D = false;
						bool hasBoxCollider2D = false;
						bool hasPointLock2D = false;

						for (YAML::const_iterator it2 = it1->second.begin(); it2 != it1->second.end(); ++it2)
						{
							if (it2->first.as<std::string>() == "name")
							{
								name = it2->second.as<std::string>();
							}
							else if (it2->first.as<std::string>() == "parent")
							{
								hasParent = true;
								parentName = it2->second.as<std::string>();
							}
							else if (it2->first.as<std::string>() == "position_px")
							{
								transform.position_px.x = it2->second[0].as<double>();
								transform.position_px.y = it2->second[1].as<double>();
							}
							else if (it2->first.as<std::string>() == "rotation_rad")
							{
								transform.rotation_rad = it2->second.as<double>();
							}
							else if (it2->first.as<std::string>() == "scale")
							{
								transform.scale.x = it2->second[0].as<double>();
								transform.scale.y = it2->second[1].as<double>();
							}
							else if (it2->first.as<std::string>() == "shader2D")
							{
								MemoryPoolU8 shaderContents = FileSystem::readString(this->dataFilePath + it2->second.as<std::string>());
								shaders2D.push_back((const char*)shaderContents.getData());
								shaderContents.deallocate();

							}
							else if (it2->first.as<std::string>() == "Camera2D")
							{
								hasCamera2D = true;
								for (YAML::const_iterator it3 = it2->second.begin(); it3 != it2->second.end(); ++it3)
								{
									if (it3->first.as<std::string>() == "viewport_px")
									{
										cameraViewport_px.x = it3->second[0].as<double>();
										cameraViewport_px.y = it3->second[1].as<double>();
									}
								}
							}
							else if (it2->first.as<std::string>() == "Sprite2D")
							{
								hasSprite2D = true;
								for (YAML::const_iterator it3 = it2->second.begin(); it3 != it2->second.end(); ++it3)
								{
									if (it3->first.as<std::string>() == "texture")
									{
										textureFrameFilePaths.push_back(it3->second.as<std::string>());
									}
								}
							}
							else if (it2->first.as<std::string>() == "RigidBody2D")
							{

							}
							else if (it2->first.as<std::string>() == "BoxCollider2D")
							{

							}
							else if (it2->first.as<std::string>() == "PointLock2D")
							{

							}
						}

						LayerID lastLayer = this->layerTypeList.size() - 1;

						this->addEntity2D(
							lastLayer,
							name,
							transform.position_px.x,
							transform.position_px.y,
							transform.rotation_rad,
							transform.scale.x,
							transform.scale.y);

						Vector<Entity2D>* entities = &this->subScenes2D.at(lastLayer).entities;
						EntityID lastEntity = this->subScenes2D.at(lastLayer).entities.size() - 1;

						entities->at(lastEntity).shaders2D = shaders2D;

						if (hasParent)
						{
							this->addParentToEntity2D(lastLayer, lastEntity, parentName);
						}
						if (hasCamera2D)
						{
							this->addCamera2D(lastLayer, lastEntity);
						}
						if (hasSprite2D)
						{
							this->addSprite2D(lastLayer, lastEntity);
							for (int i = 0; i < textureFrameFilePaths.size(); i++)
								this->addSpriteTextureFrame(
									lastLayer,
									entities->at(lastEntity).components.at(Entity2D::components::SPRITE_2D),
									this->dataFilePath + textureFrameFilePaths.at(i));
						}
					}
				}*/
			}
		}
	}
	catch (int e)
	{

	}

	return scene;
}