#include "transform.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/rotate_vector.hpp>

void Omnia::Transform::deserialize(YAML::Node yamlNode)
{
	for (YAML::const_iterator it3 = yamlNode.begin(); it3 != yamlNode.end(); ++it3)
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

void Omnia::Transform::translateX(float offset)
{
	this->translation.x += offset;
}

void Omnia::Transform::translateY(float offset)
{
	this->translation.y += offset;
}

void Omnia::Transform::translateZ(float offset)
{
	this->translation.z += offset;
}

void Omnia::Transform::rotateX(float angle)
{
	this->rotation.x += angle;
}

void Omnia::Transform::rotateY(float angle)
{
	this->rotation.y += angle;
}

void Omnia::Transform::rotateZ(float angle)
{
	this->rotation.z += angle;
}

float Omnia::Transform::calculateDistanceFrom(glm::vec3 position)
{
	return sqrt(
		pow(position.x - this->translation.x, 2) + 
		pow(position.y - this->translation.y, 2) + 
		pow(position.z - this->translation.z, 2)
	);
}

float Omnia::Transform::calculateAzimuthFrom(glm::vec3 position)
{
	return atan2(position.y - this->translation.y, position.z - this->translation.z);
}

float Omnia::Transform::calculateElevationFrom(glm::vec3 position)
{
	return atan2(position.x - this->translation.x, position.z - this->translation.z);
}

std::shared_ptr<Omnia::Transform> Omnia::Transform::getGlobalTransform()
{
	if (this->globalTransform == nullptr)
		this->globalTransform = std::shared_ptr<Transform>(new Transform());

	std::shared_ptr<Transform> rootTransform;
	int rootTransformIndex;

	if (this->componentHierarchy.size() == 0)
		this->componentHierarchy.push_back(std::shared_ptr<Transform>(this));

	for (int i = this->componentHierarchy.size() - 1;
		i >= 0;
		i--)
	{
		rootTransform = std::dynamic_pointer_cast<Transform>(this->componentHierarchy[i]);
		if (rootTransform != nullptr)
		{
			rootTransformIndex = i;
			break;
		}
	}

	this->globalTransform->translation = rootTransform->translation;
	this->globalTransform->rotation = rootTransform->rotation;
	this->globalTransform->scale = rootTransform->scale;

	for (int i = rootTransformIndex - 1; 
		i >= 0; 
		i--)
	{
		std::shared_ptr<Transform> localTransform = std::dynamic_pointer_cast<Transform>(this->componentHierarchy[i]);

		if (localTransform != nullptr)
		{
			glm::vec3 radiansRotation = glm::vec3(
				glm::radians(this->globalTransform->rotation.x),
				glm::radians(this->globalTransform->rotation.y),
				glm::radians(this->globalTransform->rotation.z)
			);

			glm::vec3 distanceVector = localTransform->translation - globalTransform->translation;
			float distance = glm::length(distanceVector);
			glm::vec3 direction = glm::normalize(distanceVector);

			if (!(std::isnan(direction.x) || std::isnan(direction.y) || std::isnan(direction.z)))
			{
				direction = glm::rotateX(direction, radiansRotation.x);
				direction = glm::rotateY(direction, radiansRotation.y);
				direction = glm::rotateZ(direction, radiansRotation.z);
				this->globalTransform->translation += direction * distance;
			}

			this->globalTransform->rotation += localTransform->rotation;
			this->globalTransform->scale *= localTransform->scale;
		}
	}

	return this->globalTransform;
}

glm::mat4 Omnia::Transform::getTransformMatrix()
{
	glm::mat4 transformMatrix = glm::translate(glm::mat4(1.0f), this->translation);
	transformMatrix = glm::rotate(transformMatrix, glm::radians(this->rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	transformMatrix = glm::rotate(transformMatrix, glm::radians(this->rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	transformMatrix = glm::rotate(transformMatrix, glm::radians(this->rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	transformMatrix = glm::scale(transformMatrix, this->scale);
	return transformMatrix;
}