#include "transform.hpp"
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <gtx/matrix_decompose.hpp>
#include <gtx/rotate_vector.hpp>

void Omnific::Transform::deserialize(YAML::Node yaml_node)
{
	for (YAML::const_iterator it3 = yaml_node.begin(); it3 != yaml_node.end(); ++it3)
	{
		if (it3->first.as<std::string>() == "translation")
		{
			this->translation.x = it3->second[0].as<double>();
			this->translation.y = it3->second[1].as<double>();
			this->translation.z = it3->second[2].as<double>();
		}
		else if (it3->first.as<std::string>() == "rotation")
		{
			this->rotation.x = it3->second[0].as<double>();
			this->rotation.y = it3->second[1].as<double>();
			this->rotation.z = it3->second[2].as<double>();
		}
		else if (it3->first.as<std::string>() == "scale")
		{
			this->scale.x = it3->second[0].as<double>();
			this->scale.y = it3->second[1].as<double>();
			this->scale.z = it3->second[2].as<double>();
		}
	}
}

void Omnific::Transform::translate_x(float offset)
{
	this->translation.x += offset;
}

void Omnific::Transform::translate_y(float offset)
{
	this->translation.y += offset;
}

void Omnific::Transform::translate_z(float offset)
{
	this->translation.z += offset;
}

void Omnific::Transform::rotate_x(float angle)
{
	this->rotation.x += angle;
}

void Omnific::Transform::rotate_y(float angle)
{
	this->rotation.y += angle;
}

void Omnific::Transform::rotate_z(float angle)
{
	this->rotation.z += angle;
}

float Omnific::Transform::calculate_distance_from(glm::vec3 position)
{
	return sqrt(
		pow(position.x - this->translation.x, 2) + 
		pow(position.y - this->translation.y, 2) + 
		pow(position.z - this->translation.z, 2)
	);
}

float Omnific::Transform::calculate_azimuth_from(glm::vec3 position)
{
	return atan2(position.y - this->translation.y, position.z - this->translation.z);
}

float Omnific::Transform::calculate_elevation_from(glm::vec3 position)
{
	return atan2(position.x - this->translation.x, position.z - this->translation.z);
}

std::shared_ptr<Omnific::Transform> Omnific::Transform::get_global_transform()
{
	if (this->global_transform == nullptr)
		this->global_transform = std::shared_ptr<Transform>(new Transform());

	std::shared_ptr<Transform> root_transform;
	int root_transform_index;

	if (this->component_hierarchy.size() == 0)
		this->component_hierarchy.push_back(std::shared_ptr<Transform>(this));

	for (int i = this->component_hierarchy.size() - 1;
		i >= 0;
		i--)
	{
		root_transform = std::dynamic_pointer_cast<Transform>(this->component_hierarchy[i]);
		if (root_transform != nullptr)
		{
			root_transform_index = i;
			break;
		}
	}

	this->global_transform->translation = root_transform->translation;
	this->global_transform->rotation = root_transform->rotation;
	this->global_transform->scale = root_transform->scale;

	for (int i = root_transform_index - 1; 
		i >= 0; 
		i--)
	{
		std::shared_ptr<Transform> local_transform = std::dynamic_pointer_cast<Transform>(this->component_hierarchy[i]);

		if (local_transform != nullptr)
		{
			glm::vec3 radians_rotation = glm::vec3(
				glm::radians(this->global_transform->rotation.x),
				glm::radians(this->global_transform->rotation.y),
				glm::radians(this->global_transform->rotation.z)
			);

			glm::vec3 distance_vector = local_transform->translation - global_transform->translation;
			float distance = glm::length(distance_vector);
			glm::vec3 direction = glm::normalize(distance_vector);

			if (!(std::isnan(direction.x) || std::isnan(direction.y) || std::isnan(direction.z)))
			{
				direction = glm::rotateX(direction, radians_rotation.x);
				direction = glm::rotateY(direction, radians_rotation.y);
				direction = glm::rotateZ(direction, radians_rotation.z);
				this->global_transform->translation += direction * distance;
			}

			this->global_transform->rotation += local_transform->rotation;
			this->global_transform->scale *= local_transform->scale;
		}
	}

	return this->global_transform;
}

glm::mat4 Omnific::Transform::get_transform_matrix()
{
	glm::mat4 transform_matrix = glm::translate(glm::mat4(1.0f), this->translation);
	transform_matrix = glm::rotate(transform_matrix, glm::radians(this->rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	transform_matrix = glm::rotate(transform_matrix, glm::radians(this->rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	transform_matrix = glm::rotate(transform_matrix, glm::radians(this->rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	transform_matrix = glm::scale(transform_matrix, this->scale);
	return transform_matrix;
}