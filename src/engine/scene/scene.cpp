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
#include "scene/components/component.hpp"
#include <foundations/singletons/platform/platform.hpp>
#include <yaml-cpp/yaml.h>
#include <customization/class_registry.hpp>
#include <gtx/quaternion.hpp>
#include <cmath>
#include <foundations/singletons/profiler.hpp>

Omnific::Scene::Scene()
{
	this->id = UIDGenerator::get_new_uid();
}

Omnific::Scene::Scene(std::string filepath)
{
	this->deserialize_from(filepath);
	this->id = UIDGenerator::get_new_uid();
}

void Omnific::Scene::serialize_to(std::string filepath)
{

}

void Omnific::Scene::deserialize_from(std::string filepath)
{
	FileAccess& file_access = Platform::get_file_access();
	Platform::get_logger().write("Loading Scene from: \"" + filepath + "\"");
	this->name = filepath;

	if (filepath != "")
	{
		const std::string full_filepath = Platform::get_file_access().find_path(filepath);
		const std::string file_extension = file_access.get_file_extension(filepath);

		if (file_extension == "yml" || file_extension == "yaml")
		{
			try
			{
				YAML::Node yaml_node = YAML::LoadFile(full_filepath);

				for (YAML::const_iterator it0 = yaml_node.begin(); it0 != yaml_node.end(); ++it0)
				{
					if (it0->first.as<std::string>() == "SceneLayer")
					{
						std::shared_ptr<SceneLayer> scene_layer(new SceneLayer());

						for (YAML::const_iterator it1 = it0->second.begin(); it1 != it0->second.end(); ++it1)
						{
							if (it1->first.as<std::string>() == "name")
							{
								scene_layer->name = it1->second.as<std::string>();
							}
							else if (it1->first.as<std::string>() == "spatial_dimension")
							{
								int value = it1->second.as<int>();

								if (value == 2)
									scene_layer->is_2d = true;
								else if (value == 3)
									scene_layer->is_2d = false;
							}
							else if (it1->first.as<std::string>() == "Entity" || it1->first.as<std::string>() == "Scene")
							{
								std::shared_ptr<Entity> entity(new Entity());
								scene_layer->add_entity(entity);
								std::string name;

								for (YAML::const_iterator it2 = it1->second.begin(); it2 != it1->second.end(); ++it2)
								{
									//Entity attributes
									if (it2->first.as<std::string>() == "name")
									{
										name = it2->second.as<std::string>();
										scene_layer->set_entity_name(scene_layer->get_last_entity()->get_id(), name);
									}
									else if (it2->first.as<std::string>() == "parent")
									{
										scene_layer->add_entity_to_parent_entity_by_name(entity, it2->second.as<std::string>());
									}
									else if ((it2->first.as<std::string>() == "transform"))
									{
										for (YAML::const_iterator it3 = it2->second.begin(); it3 != it2->second.end(); ++it3)
										{
											std::shared_ptr<Transform> transform = entity->get_transform();

											if (it3->first.as<std::string>() == "translation")
											{
												transform->translation.x = it3->second[0].as<double>();
												transform->translation.y = it3->second[1].as<double>();
												transform->translation.z = it3->second[2].as<double>();
											}
											else if (it3->first.as<std::string>() == "rotation")
											{
												transform->rotation.x = it3->second[0].as<double>();
												transform->rotation.y = it3->second[1].as<double>();
												transform->rotation.z = it3->second[2].as<double>();
											}
											else if (it3->first.as<std::string>() == "scale")
											{
												transform->scale.x = it3->second[0].as<double>();
												transform->scale.y = it3->second[1].as<double>();
												transform->scale.z = it3->second[2].as<double>();
											}
										}					
									}
									else
									{
										//Components
										std::shared_ptr<Registerable> registerable = ClassRegistry::query<Component>(it2->first.as<std::string>());

										if (registerable != nullptr)
										{
											std::shared_ptr<Component> component = std::dynamic_pointer_cast<Component>(std::shared_ptr<Registerable>(registerable->instance()));
											component->deserialize(it2->second);
											scene_layer->add_component_to_last_entity(component);
										}
									}
								}

								if (it1->first.as<std::string>() == "Scene")
								{
									if (name != filepath)
									{
										std::shared_ptr<Scene> other_scene = std::shared_ptr<Scene>(new Scene(name));

										for (const auto& [id, other_scene_layer] : other_scene->get_scene_layers())
											scene_layer->merge_another_scene_layer_to_parent_entity(other_scene_layer, entity->get_id());
									}
								}
							}
						}

						this->add_scene_layer(scene_layer);
					}
				}
			}
			catch (int e)
			{

			}

			std::shared_ptr<SceneLayer> debug_scene_layer = std::make_shared<SceneLayer>();
			std::shared_ptr<Entity> debug_gui_entity = std::make_shared<Entity>();
			std::shared_ptr<GUI> debug_gui = std::make_shared<GUI>();
			std::shared_ptr<Camera> debug_camera = std::make_shared<Camera>();

			debug_gui->hide();
			debug_gui->set_to_label("");
			debug_scene_layer->add_empty_entity();
			debug_scene_layer->add_component_to_last_entity(debug_camera);
			debug_scene_layer->add_entity(debug_gui_entity);
			debug_scene_layer->add_component_to_last_entity(debug_gui);
			debug_scene_layer->name = "debug_scene_layer";
			debug_scene_layer->set_entity_name(debug_gui_entity->get_id(), "debug_gui_entity");
			this->add_scene_layer(debug_scene_layer);
		}
		else
		{
			this->add_scene_layer(std::shared_ptr<SceneLayer>(new SceneLayer(filepath)));
		}
	}
}

void Omnific::Scene::reload()
{
	this->scene_layers.clear();
	this->last_scene_layer_id = 0;
	this->deserialize_from(this->get_name());
}

void Omnific::Scene::add_scene_layer(std::shared_ptr<SceneLayer> scene_layer)
{
	this->scene_layers.emplace(scene_layer->get_id(), scene_layer);
	this->last_scene_layer_id = scene_layer->get_id();
}

void Omnific::Scene::add_empty_scene_layer()
{
	std::shared_ptr<SceneLayer> scene_layer = std::shared_ptr<SceneLayer>(new SceneLayer());
	this->add_scene_layer(scene_layer);
}

void Omnific::Scene::remove_scene_layer(SceneLayerID scene_layer_id)
{
	if (this->scene_layers.count(scene_layer_id))
		this->scene_layers.erase(scene_layer_id);
}

std::string Omnific::Scene::get_name()
{
	return this->name;
}

std::shared_ptr<Omnific::SceneLayer> Omnific::Scene::get_scene_layer(SceneLayerID scene_layer)
{
	return this->scene_layers.at(scene_layer);
}

std::shared_ptr<Omnific::SceneLayer> Omnific::Scene::get_scene_layer_by_name(std::string name)
{
	std::shared_ptr<SceneLayer> scene_layer = nullptr;

	for (auto it = this->scene_layers.begin(); it != this->scene_layers.end(); it++)
		if (it->second->get_name() == name)
			return it->second;

	return scene_layer;
}

std::shared_ptr<Omnific::SceneLayer> Omnific::Scene::get_last_scene_layer()
{
	return this->scene_layers.at(this->last_scene_layer_id);
}

std::map<Omnific::SceneLayerID, std::shared_ptr<Omnific::SceneLayer>>& Omnific::Scene::get_scene_layers()
{
	return this->scene_layers;
}

Omnific::SceneID Omnific::Scene::get_id()
{
	return this->id;
}

void Omnific::Scene::update_debug_scene_layer()
{
	std::string debug_string = Profiler::get_clock_deltas_to_string_by_tag("total");
	std::shared_ptr<SceneLayer> debug_scene_layer = this->get_scene_layer_by_name("debug_scene_layer");
	std::shared_ptr<GUI> debug_gui = debug_scene_layer->get_component_by_type<GUI>(debug_scene_layer->get_entity_by_name("debug_gui_entity")->get_id());
	Inputs& inputs = Platform::get_inputs();

	if (debug_gui->get_alpha_in_percentage() > 0.1)
	{
		debug_string += Profiler::get_rendering_backend_name();
		debug_gui->set_to_label(debug_string);
	}

	if (inputs.is_on_release("f3"))
	{
		if (debug_gui->get_alpha_in_percentage() < 0.5)
		{
			debug_gui->show();
		}
		else
		{
			debug_gui->hide();
		}
	}
}