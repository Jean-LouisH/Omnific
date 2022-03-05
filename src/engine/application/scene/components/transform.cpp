#include "transform.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


glm::mat4 Omnia::Transform::getGlobalTransformMatrix()
{
	glm::mat4 globalTransform = glm::translate(glm::mat4(1.0f), this->translation);
	globalTransform = glm::rotate(globalTransform, glm::radians(this->rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	globalTransform = glm::rotate(globalTransform, glm::radians(this->rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	globalTransform = glm::rotate(globalTransform, glm::radians(this->rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	globalTransform = glm::scale(globalTransform, this->scale);
	return globalTransform;
}