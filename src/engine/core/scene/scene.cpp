// MIT License
// 
// Copyright (c) 2020 Jean-Louis Haywood
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "scene.hpp"
#include "core/component.hpp"
#include <core/singletons/os/os.hpp>
#include <yaml-cpp/yaml.h>
#include <customization/class_registry/class_registry.hpp>
#include <glm/gtx/quaternion.hpp>
#include <cmath>

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <tiny_gltf.h>

Omnia::Scene::Scene()
{
	this->id = UIDGenerator::getNewUID();
}

Omnia::Scene::Scene(std::string filepath)
{
	this->deserialize(filepath);
	this->id = UIDGenerator::getNewUID();
}

Omnia::Scene::Scene(std::string filepath, std::string name)
{
	this->deserialize(filepath, name);
	this->id = UIDGenerator::getNewUID();
}

void Omnia::Scene::serialize(std::string filepath)
{

}

void Omnia::Scene::deserialize(std::string filepath)
{
	return this->deserialize(filepath, "");
}

void Omnia::Scene::deserialize(std::string filepath, std::string name)
{
	if (name == "")
		OS::getLogger().write("Loading all SceneTrees from Scene: \"" + filepath + "\"");
	else
		OS::getLogger().write("Loading SceneTree: \"" + name + "\" from Scene : \"" + filepath + "\"");


	this->name = filepath;
	const std::string fullFilepath = OS::getFileAccess().getDataDirectoryPath() + filepath;

	try
	{
		YAML::Node yamlNode = YAML::LoadFile(fullFilepath);

		for (YAML::const_iterator it0 = yamlNode.begin(); it0 != yamlNode.end(); ++it0)
		{
			if (it0->first.as<std::string>() == "SceneTree")
			{
				std::shared_ptr<SceneTree> sceneTree(new SceneTree());
				bool loadThisSceneTree = true;

				/* If the name is an empty string, load all, otherwise search for the name */
				if (name != "")
				{
					loadThisSceneTree = false;

					for (YAML::const_iterator it1 = it0->second.begin(); it1 != it0->second.end(); ++it1)
					{
						if (it1->first.as<std::string>() == "name")
						{
							loadThisSceneTree = it1->second.as<std::string>() == name;
							break;
						}
					}
				}

				if (loadThisSceneTree)
				{
					for (YAML::const_iterator it1 = it0->second.begin(); it1 != it0->second.end(); ++it1)
					{
						if (it1->first.as<std::string>() == "name")
						{
							sceneTree->name = it1->second.as<std::string>();
						}
						else if (it1->first.as<std::string>() == "spatial_dimension")
						{
							int value = it1->second.as<int>();

							if (value == 2)
								sceneTree->is2D = true;
							else if (value == 3)
								sceneTree->is2D = false;
						}
						else if (it1->first.as<std::string>() == "Entity")
						{
							std::shared_ptr<Entity> entity(new Entity());
							sceneTree->addEntity(entity);

							for (YAML::const_iterator it2 = it1->second.begin(); it2 != it1->second.end(); ++it2)
							{
								//Entity attributes
								if (it2->first.as<std::string>() == "name")
								{
									sceneTree->setEntityName(sceneTree->getLastEntity()->getID(), it2->second.as<std::string>());
								}
								else if (it2->first.as<std::string>() == "parent")
								{
									sceneTree->getLastEntity()->parentID = sceneTree->getEntityByName(it2->second.as<std::string>())->getID();
								}
								//Components
								else
								{
									std::shared_ptr<Registerable> registerable = ClassRegistry::query<Component>(it2->first.as<std::string>());

									if (registerable != nullptr)
									{
										std::shared_ptr<Component> component = std::dynamic_pointer_cast<Component>(std::shared_ptr<Registerable>(registerable->instance()));
										component->deserialize(it2->second);
										sceneTree->addComponentToLastEntity(component);
									}
								}
							}
						}
						/* Recursively load another SceneTree into this one if the filename
						   is not the same. */
						else if (it1->first.as<std::string>() == "SubSceneTree")
						{
							std::string subSceneFilepath;
							std::string subSceneTreeName;
							EntityID parentID = 0;

							for (YAML::const_iterator it2 = it1->second.begin(); it2 != it1->second.end(); ++it2)
							{
								if (it2->first.as<std::string>() == "parent")
								{
									parentID = sceneTree->getEntityByName(it2->second.as<std::string>())->getID();
								}
								else if (it2->first.as<std::string>() == "name")
								{
									subSceneFilepath = it2->second[0].as<std::string>();
									subSceneTreeName = it2->second[1].as<std::string>();
								}
							}

							std::shared_ptr<SceneTree> subSceneTree;

							if (subSceneFilepath != filepath)
							{
								/*The last SceneTree is the only SceneTree, so this just extracts the one specified.*/
								if (subSceneTreeName != "")
									subSceneTree = Scene(subSceneFilepath, subSceneTreeName).getLastSceneTree();
								else
									subSceneTree = this->loadGLTF(subSceneFilepath);


								/* Only load the SceneTree if it is the same spatial dimension. */
								if (subSceneTree->is2D == sceneTree->is2D)
								{
									/* Transfer Entities and their Components */
									std::shared_ptr<Entity> newRootEntity(new Entity());
									newRootEntity->setName(subSceneFilepath);
									newRootEntity->parentID = parentID;
									sceneTree->addEntity(newRootEntity);
									std::unordered_map<EntityID, std::shared_ptr<Entity>>& subSceneEntities = subSceneTree->getEntities();

									/*Entities without parents are listed before others.*/
									std::vector<std::shared_ptr<Entity>> sortedEntities;


									/*Without parents*/
									for (auto it = subSceneEntities.begin(); it != subSceneEntities.end(); it++)
									{
										std::shared_ptr<Entity> subSceneEntity = it->second;

										if (subSceneEntity->parentID == 0)
											sortedEntities.push_back(subSceneEntity);
									}

									/*With parents*/

									for (auto it = subSceneEntities.begin(); it != subSceneEntities.end(); it++)
									{
										std::shared_ptr<Entity> subSceneEntity = it->second;

										if (subSceneEntity->parentID != 0)
											sortedEntities.push_back(subSceneEntity);
									}


									for (size_t i = 0; i < sortedEntities.size(); i++)
									{
										std::shared_ptr<Entity> subSceneEntity = sortedEntities[i];

										if (subSceneEntity->parentID == 0)
											subSceneEntity->parentID = newRootEntity->getID();

										sceneTree->addEntity(subSceneEntity);

										std::unordered_map<std::string, ComponentID> subSceneEntityComponentIDs = subSceneEntity->componentIDs;

										for (auto it2 = subSceneEntityComponentIDs.begin(); it2 != subSceneEntityComponentIDs.end(); it2++)
											sceneTree->addComponentToLastEntity(subSceneTree->getComponentByID(it2->second));
									}
								}
							}
						}
					}
				}

				this->addSceneTree(sceneTree);
			}
		}
	}
	catch (int e)
	{

	}
}

void Omnia::Scene::reload()
{
	this->sceneTrees.clear();
	this->lastSceneTreeID = 0;
	this->deserialize(this->getName());
}

void Omnia::Scene::addSceneTree(std::shared_ptr<SceneTree> sceneTree)
{
	this->sceneTrees.emplace(sceneTree->getID(), sceneTree);
	this->lastSceneTreeID = sceneTree->getID();
}

void Omnia::Scene::addEmptySceneTree()
{
	std::shared_ptr<SceneTree> sceneTree = std::shared_ptr<SceneTree>(new SceneTree());
	this->addSceneTree(sceneTree);
}

void Omnia::Scene::removeSceneTree(SceneTreeID sceneTreeID)
{
	if (this->sceneTrees.count(sceneTreeID))
		this->sceneTrees.erase(sceneTreeID);
}

std::string Omnia::Scene::getName()
{
	return this->name;
}

std::shared_ptr<Omnia::SceneTree> Omnia::Scene::getSceneTree(SceneTreeID sceneTree)
{
	return this->sceneTrees.at(sceneTree);
}

std::shared_ptr<Omnia::SceneTree> Omnia::Scene::getSceneTreeByName(std::string name)
{
	std::shared_ptr<SceneTree> sceneTree = nullptr;

	for (auto it = this->sceneTrees.begin(); it != this->sceneTrees.end(); it++)
		if (it->second->getName() == name)
			return it->second;

	return sceneTree;
}

std::shared_ptr<Omnia::SceneTree> Omnia::Scene::getLastSceneTree()
{
	return this->sceneTrees.at(this->lastSceneTreeID);
}

std::unordered_map<Omnia::SceneTreeID, std::shared_ptr<Omnia::SceneTree>>& Omnia::Scene::getSceneTrees()
{
	return this->sceneTrees;
}

Omnia::SceneID Omnia::Scene::getID()
{
	return this->id;
}

std::shared_ptr<Omnia::SceneTree> Omnia::Scene::loadGLTF(std::string filepath)
{
	std::shared_ptr<SceneTree> sceneTree(new SceneTree());

	tinygltf::Model gltfData;
	tinygltf::TinyGLTF tinyGLTF;
	std::string err;
	std::string warn;

	bool ret = tinyGLTF.LoadBinaryFromFile(
		&gltfData,
		&err, 
		&warn, 
		OS::getFileAccess().getDataDirectoryPath() + filepath);

	if (!warn.empty())
		printf("Warn: %s\n", warn.c_str());

	if (!err.empty())
		printf("Err: %s\n", err.c_str());

	if (!ret)
	{
		printf("Failed to parse glTF\n");
	}
	else
	{
		for (size_t i = 0; i < gltfData.scenes.size(); i++)
		{
			tinygltf::Scene gltfScene = gltfData.scenes[i];
			std::shared_ptr<Entity> gltfSceneRootEntity(new Entity());
			sceneTree->addEntity(gltfSceneRootEntity);

			for (size_t j = 0; j < gltfScene.nodes.size(); j++)
			{
				int nodeIndex = gltfScene.nodes[j];
				tinygltf::Node gltfNode = gltfData.nodes[nodeIndex];
				int meshIndex = gltfNode.mesh;

				// GLTF data
				std::vector<float> positions = this->readGLTFPrimitiveAttribute(gltfData, "POSITION", meshIndex);
				std::vector<float> textureCoords = this->readGLTFPrimitiveAttribute(gltfData, "TEXCOORD_0", meshIndex);
				std::vector<float> normals = this->readGLTFPrimitiveAttribute(gltfData, "NORMAL", meshIndex);
				std::vector<uint32_t> indices = this->readGLTFPrimitiveIndices(gltfData, meshIndex);

				std::shared_ptr<Mesh> mesh(new Mesh(positions, textureCoords, normals, indices));
				std::shared_ptr<Material> material(new Material());

				std::shared_ptr<Entity> entity(new Entity());
				entity->parentID = gltfSceneRootEntity->getID();
				sceneTree->addEntity(entity);
				std::shared_ptr<Transform> transform(new Transform());
				std::shared_ptr<Model> model(new Model());

				int materialIndex = gltfData.meshes.at(meshIndex).primitives.at(0).material;

				if (materialIndex != -1)
				{
					tinygltf::Material gltfMaterial = gltfData.materials.at(gltfData.meshes.at(meshIndex).primitives.at(0).material);
					int baseColourTextureIndex = gltfMaterial.pbrMetallicRoughness.baseColorTexture.index;
					int metallicRougnessTextureIndex = gltfMaterial.pbrMetallicRoughness.metallicRoughnessTexture.index;
					int normalTextureIndex = gltfMaterial.normalTexture.index;
					int emissiveTextureIndex = gltfMaterial.emissiveTexture.index;

					/* Albedo / BaseColour*/
					if (baseColourTextureIndex != -1)
					{
						tinygltf::Image gltfImage = gltfData.images.at(baseColourTextureIndex);
						tinygltf::BufferView bufferView = gltfData.bufferViews.at(gltfImage.bufferView);
						std::vector<unsigned char> buffer = gltfData.buffers.at(bufferView.buffer).data;
						std::vector<uint8_t> imageFileBytes = this->readGLTFBuffer(buffer, bufferView);
						int width = 0;
						int height = 0;
						int colourChannels = 0;

						stbi_set_flip_vertically_on_load(0);

						uint8_t* imageData = stbi_load_from_memory(
							imageFileBytes.data(),
							imageFileBytes.size(),
							&width,
							&height,
							&colourChannels,
							0);

						material->albedo = std::shared_ptr<Image>(new Image(
							imageData,
							width,
							height,
							colourChannels));
					}
					else
					{
						std::vector<double> baseColorFactor = gltfMaterial.pbrMetallicRoughness.baseColorFactor;

						if (baseColorFactor.size() == 3)
						{
							material->albedo = std::shared_ptr<Image>(new Image(std::shared_ptr<Colour>(new Colour(
								baseColorFactor[0],
								baseColorFactor[1],
								baseColorFactor[2],
								1.0))
							));
						}
						else if (baseColorFactor.size() == 4)
						{
							material->albedo = std::shared_ptr<Image>(new Image(std::shared_ptr<Colour>(new Colour(
								baseColorFactor[0],
								baseColorFactor[1],
								baseColorFactor[2],
								baseColorFactor[3]))
							));
						}
					}

					/* Metallicity and Roughness */
					if (metallicRougnessTextureIndex != -1)
					{

					}
					else
					{
						double metallicFactor = gltfMaterial.pbrMetallicRoughness.metallicFactor;
						double roughnessFactor = gltfMaterial.pbrMetallicRoughness.roughnessFactor;

						material->metallicity = std::shared_ptr<Image>(new Image(std::shared_ptr<Colour>(new Colour(
							metallicFactor,
							metallicFactor,
							metallicFactor,
							1.0))
						));

						material->roughness = std::shared_ptr<Image>(new Image(std::shared_ptr<Colour>(new Colour(
							roughnessFactor,
							roughnessFactor,
							roughnessFactor,
							1.0))
						));

					}

					/* Normal Map*/
					if (normalTextureIndex != -1)
					{
						tinygltf::Image gltfImage = gltfData.images[normalTextureIndex];

						tinygltf::BufferView bufferView = gltfData.bufferViews.at(gltfImage.bufferView);
						std::vector<unsigned char> buffer = gltfData.buffers.at(bufferView.buffer).data;
						std::vector<uint8_t> imageData = this->readGLTFBuffer(buffer, bufferView);

						material->normal = std::shared_ptr<Image>(new Image(
							imageData.data(), 
							gltfImage.width, 
							gltfImage.height,
							gltfImage.component));
					}
					else
					{
						material->normal = std::shared_ptr<Image>(new Image(std::shared_ptr<Colour>(new Colour(
							0.0,
							0.0,
							0.0,
							1.0))
						));
					}

					/* Emission */
					if (emissiveTextureIndex != -1)
					{
						tinygltf::Image gltfImage = gltfData.images[emissiveTextureIndex];

						tinygltf::BufferView bufferView = gltfData.bufferViews.at(gltfImage.bufferView);
						std::vector<unsigned char> buffer = gltfData.buffers.at(bufferView.buffer).data;
						std::vector<uint8_t> imageData = this->readGLTFBuffer(buffer, bufferView);

						material->emission = std::shared_ptr<Image>(new Image(
							imageData.data(),
							gltfImage.width,
							gltfImage.height,
							gltfImage.component));
					}
					else
					{
						std::vector<double> emissiveFactor = gltfMaterial.emissiveFactor;

						material->emission = std::shared_ptr<Image>(new Image(std::shared_ptr<Colour>(new Colour(
							emissiveFactor[0],
							emissiveFactor[1],
							emissiveFactor[2],
							1.0))
						));
					}

				}
				
				if (gltfNode.translation.size() == 3)
				{
					transform->translation = { 
						gltfNode.translation[0], 
						gltfNode.translation[1], 
						gltfNode.translation[2] };
				}

				if (gltfNode.rotation.size() == 4)
				{
					glm::quat unitQuaternion = glm::quat(
						gltfNode.rotation[3],
						gltfNode.rotation[0],
						gltfNode.rotation[1],
						gltfNode.rotation[2]);

					transform->rotation = glm::eulerAngles(unitQuaternion);
					transform->rotation *= 180.0 / M_PI;
				}

				if (gltfNode.scale.size() == 3)
				{
					transform->scale = {
						gltfNode.scale[0],
						gltfNode.scale[1],
						gltfNode.scale[2] };
				}

				model->material = material;
				model->mesh = mesh;

				sceneTree->addComponentToLastEntity(std::dynamic_pointer_cast<Component>(transform));
				sceneTree->addComponentToLastEntity(std::dynamic_pointer_cast<Component>(model));
			}
		}
	}

	return sceneTree;
}

std::vector<uint8_t> Omnia::Scene::readGLTFBuffer(std::vector<unsigned char> bufferData, tinygltf::BufferView bufferView)
{
	std::vector<uint8_t> bytes(
		bufferData.begin() + bufferView.byteOffset,
		bufferData.begin() + bufferView.byteOffset + bufferView.byteLength);

	return bytes;
}

std::vector<float> Omnia::Scene::readGLTFPrimitiveAttribute(tinygltf::Model model, std::string attributeName, size_t index)
{
	tinygltf::Primitive primitive = model.meshes.at(index).primitives.at(0);
	tinygltf::Accessor accessor = model.accessors.at(primitive.attributes.at(attributeName));
	tinygltf::BufferView bufferView = model.bufferViews.at(accessor.bufferView);
	std::vector<unsigned char> buffer = model.buffers.at(bufferView.buffer).data;
	std::vector<uint8_t> bytes = this->readGLTFBuffer(buffer, bufferView);
	std::vector<float> attribute;
	float* floatByteData = (float*)bytes.data();
	size_t floatByteSize = bytes.size() / sizeof(float);

	for (size_t i = 0; i < floatByteSize; i++)
		attribute.push_back(floatByteData[i]);

	return attribute;
}

std::vector<uint32_t> Omnia::Scene::readGLTFPrimitiveIndices(tinygltf::Model model, size_t index)
{
	tinygltf::Primitive primitive = model.meshes.at(index).primitives.at(0);
	tinygltf::Accessor accessor = model.accessors.at(primitive.indices);
	tinygltf::BufferView bufferView = model.bufferViews.at(accessor.bufferView);
	std::vector<unsigned char> buffer = model.buffers.at(bufferView.buffer).data;
	std::vector<uint8_t> indexBytes = this->readGLTFBuffer(buffer, bufferView);
	std::vector<uint32_t> indices;

	uint16_t* shortIndexByteData = (uint16_t*)indexBytes.data();
	size_t shortIndexByteSize = indexBytes.size() / sizeof(uint16_t);

	for (size_t i = 0; i < shortIndexByteSize; i++)
		indices.push_back((uint32_t)shortIndexByteData[i]);

	return indices;
}