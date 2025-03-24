#include "transform.hpp"
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <gtx/matrix_decompose.hpp>
#include <gtx/rotate_vector.hpp>
#include <gtx/euler_angles.hpp>


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

void Omnific::Transform::flatten_to_2d()
{
	this->translation.z = 0.0;
	this->rotation.x = 0.0;
	this->rotation.y = 0.0;
	this->scale.z = 0.0;
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

glm::vec3 Omnific::Transform::get_up_vector()
{
	glm::mat4 rotation_matrix = glm::yawPitchRoll(this->rotation.y, this->rotation.x, this->rotation.z);
	return glm::vec3(rotation_matrix * glm::vec4(0, 1, 0, 0));
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