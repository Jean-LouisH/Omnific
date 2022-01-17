#include "transform.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


glm::mat4 Esi::Transform::getGlobalTransformMatrix()
{
	glm::mat4 globalTransform;
	globalTransform = glm::translate(globalTransform, this->translation);
	globalTransform = glm::rotate(globalTransform, glm::radians(this->rotation.x), glm::vec3(1.0, 0.0, 0.0));
	globalTransform = glm::rotate(globalTransform, glm::radians(this->rotation.y), glm::vec3(0.0, 1.0, 0.0));
	globalTransform = glm::rotate(globalTransform, glm::radians(this->rotation.z), glm::vec3(0.0, 0.0, 1.0));
	globalTransform = glm::scale(globalTransform, this->scale);
	return globalTransform;
}