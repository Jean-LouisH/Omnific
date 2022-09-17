#include "transform.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

void Omnia::Transform::globallyTranslateX(float offset)
{
	this->translation.x += offset;
}

void Omnia::Transform::globallyTranslateY(float offset)
{
	this->translation.y += offset;
}

void Omnia::Transform::globallyTranslateZ(float offset)
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

glm::mat4 Omnia::Transform::getTransformMatrix()
{
	glm::mat4 transformMatrix = glm::translate(glm::mat4(1.0f), this->translation);
	transformMatrix = glm::rotate(transformMatrix, glm::radians(this->rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	transformMatrix = glm::rotate(transformMatrix, glm::radians(this->rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	transformMatrix = glm::rotate(transformMatrix, glm::radians(this->rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	transformMatrix = glm::scale(transformMatrix, this->scale);
	return transformMatrix;
}