#include "transform.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/matrix_decompose.hpp>


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

void Omnific::Transform::rotate(glm::vec3 angles)
{
	this->rotate_x(angles.x);
	this->rotate_y(angles.y);
	this->rotate_z(angles.z);
}

void Omnific::Transform::rotate_x(float angle)
{
	this->rotation = glm::angleAxis(glm::radians(angle), glm::vec3(1, 0, 0)) * this->rotation;
}

void Omnific::Transform::rotate_y(float angle)
{
	this->rotation = glm::angleAxis(glm::radians(angle), glm::vec3(0, 1, 0)) * this->rotation;
}

void Omnific::Transform::rotate_z(float angle)
{
	this->rotation = glm::angleAxis(glm::radians(angle), glm::vec3(0, 0, 1)) * this->rotation;
}

void Omnific::Transform::rotate_around(glm::vec3 pivot, glm::vec3 angles)
{
    glm::vec3 offset = this->translation - pivot;
    glm::quat qX = glm::angleAxis(glm::radians(angles.x), glm::vec3(1, 0, 0));
    glm::quat qY = glm::angleAxis(glm::radians(angles.y), glm::vec3(0, 1, 0));
    glm::quat qZ = glm::angleAxis(glm::radians(angles.z), glm::vec3(0, 0, 1));
    glm::quat rotation_to_apply = qX * qY * qZ; 

    this->translation = pivot + (rotation_to_apply * offset);
	this->rotation = rotation_to_apply * this->rotation;
}

void Omnific::Transform::rotate_x_around(glm::vec3 pivot, float angle)
{
	this->rotate_around(pivot, glm::vec3(angle, 0.0, 0.0));
}

void Omnific::Transform::rotate_y_around(glm::vec3 pivot, float angle)
{
	this->rotate_around(pivot, glm::vec3(0.0, angle, 0.0));
}

void Omnific::Transform::rotate_z_around(glm::vec3 pivot, float angle)
{
	this->rotate_around(pivot, glm::vec3(0.0, 0.0, angle));
}

void Omnific::Transform::look_at(glm::vec3 position, glm::vec3 up_vector)
{
	glm::vec3 forward_vector = glm::normalize(position);
	glm::vec3 right_vector = glm::normalize(glm::cross(up_vector, forward_vector));
	glm::vec3 new_up_vector = glm::cross(forward_vector, right_vector);

	glm::mat4 rotation_matrix = glm::mat4(
		glm::vec4(right_vector, 0.0f),
		glm::vec4(new_up_vector, 0.0f),
		glm::vec4(forward_vector, 0.0f),
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
	);

	this->rotation = glm::quat_cast(rotation_matrix);
}

void Omnific::Transform::interpolate_with_transform(std::shared_ptr<Transform> target_transform, float interpolation_delta)
{
	glm::mat4 transform_matrix_from = this->get_transform_matrix();
	glm::mat4 transform_matrix_to = target_transform->get_transform_matrix();

	glm::vec3 scale_from;
	glm::quat rotation_from;
	glm::vec3 translation_from;
	glm::vec3 skew_from;
	glm::vec4 perspective_from;

	glm::decompose(transform_matrix_from, scale_from, rotation_from, translation_from, skew_from, perspective_from);

	glm::vec3 scale_to;
	glm::quat rotation_to;
	glm::vec3 translation_to;
	glm::vec3 skew_to;
	glm::vec4 perspective_to;

	glm::decompose(transform_matrix_to, scale_to, rotation_to, translation_to, skew_to, perspective_to);

	glm::vec3 interpolated_translation = glm::mix(translation_from, translation_to, interpolation_delta);
	glm::quat interpolated_rotation = glm::slerp(rotation_from, rotation_to, interpolation_delta);
	glm::vec3 interpolated_scale = glm::mix(scale_from, scale_to, interpolation_delta);
	glm::mat4 interpolated_matrix = glm::mat4(1.0);
	interpolated_matrix = glm::translate(interpolated_matrix, interpolated_translation);
	interpolated_matrix *= glm::mat4_cast(interpolated_rotation);
	interpolated_matrix = glm::scale(interpolated_matrix, interpolated_scale);

	glm::vec3 scale;
	glm::quat rotation;
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;

	glm::decompose(interpolated_matrix, scale, rotation, translation, skew, perspective);

	glm::vec3 euler_angles = glm::degrees(glm::eulerAngles(rotation));
	
	this->translation = translation;
	this->rotation = euler_angles;
	this->scale = scale;
}

void Omnific::Transform::interpolate_with_translation(glm::vec3 target_translation, float interpolation_delta)
{
	this->translation = glm::mix(this->translation, target_translation, interpolation_delta);
}

void Omnific::Transform::interpolate_with_rotation(glm::quat target_rotation, float interpolation_delta)
{
	this->rotation = glm::slerp(this->rotation, target_rotation, interpolation_delta);
}

void Omnific::Transform::interpolate_with_scale(glm::vec3 target_scale, float interpolation_delta)
{
	this->scale = glm::mix(this->scale, target_scale, interpolation_delta);
}

void Omnific::Transform::set_xyz_scale(float amount)
{
	this->scale.x = amount;
	this->scale.y = amount;
	this->scale.z = amount; 
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
	glm::mat4 rotation_matrix = glm::mat4_cast(this->rotation);
	return glm::vec3(rotation_matrix * glm::vec4(0, 1, 0, 0));
}

glm::vec3 Omnific::Transform::get_front_vector()
{
	glm::mat4 rotation_matrix = glm::mat4_cast(this->rotation);
	return glm::vec3(rotation_matrix * glm::vec4(0, 0, 1, 0));
}

glm::vec3 Omnific::Transform::get_right_vector()
{
	glm::mat4 rotation_matrix = glm::mat4_cast(this->rotation);
	return glm::vec3(rotation_matrix * glm::vec4(1, 0, 0, 0));
}

glm::vec3 Omnific::Transform::get_rotation_in_euler_angles()
{
	return this->get_rotation_in_radians_euler_angles();
}

glm::vec3 Omnific::Transform::get_rotation_in_radians_euler_angles()
{
	return glm::eulerAngles(this->rotation);
}

glm::vec3 Omnific::Transform::get_rotation_in_degrees_euler_angles()
{
	return glm::degrees(this->get_rotation_in_radians_euler_angles());
}

glm::mat4 Omnific::Transform::get_transform_matrix()
{
	glm::mat4 transform_matrix = glm::translate(glm::mat4(1.0f), this->translation);
	transform_matrix *= glm::mat4_cast(this->rotation);
	transform_matrix = glm::scale(transform_matrix, this->scale);
	return transform_matrix;
}

void Omnific::Transform::set_rotation(glm::vec3 angles)
{
	this->set_x_rotation(angles.x);
	this->set_y_rotation(angles.y);
	this->set_z_rotation(angles.z);
}

void Omnific::Transform::set_x_rotation(float angle)
{
	glm::vec3 euler_rotations = glm::eulerAngles(this->rotation);
	euler_rotations.x = glm::radians(angle);
	glm::quat qX = glm::angleAxis(euler_rotations.x, glm::vec3(1, 0, 0));
    glm::quat qY = glm::angleAxis(euler_rotations.y, glm::vec3(0, 1, 0));
    glm::quat qZ = glm::angleAxis(euler_rotations.z, glm::vec3(0, 0, 1));
    this->rotation = qX * qY * qZ; 
}

void Omnific::Transform::set_y_rotation(float angle)
{
	glm::vec3 euler_rotations = glm::eulerAngles(this->rotation);
	euler_rotations.y = glm::radians(angle);
	glm::quat qX = glm::angleAxis(euler_rotations.x, glm::vec3(1, 0, 0));
    glm::quat qY = glm::angleAxis(euler_rotations.y, glm::vec3(0, 1, 0));
    glm::quat qZ = glm::angleAxis(euler_rotations.z, glm::vec3(0, 0, 1));
    this->rotation = qX * qY * qZ; 
}

void Omnific::Transform::set_z_rotation(float angle)
{
	glm::vec3 euler_rotations = glm::eulerAngles(this->rotation);
	euler_rotations.z = glm::radians(angle);
	glm::quat qX = glm::angleAxis(euler_rotations.x, glm::vec3(1, 0, 0));
    glm::quat qY = glm::angleAxis(euler_rotations.y, glm::vec3(0, 1, 0));
    glm::quat qZ = glm::angleAxis(euler_rotations.z, glm::vec3(0, 0, 1));
    this->rotation = qX * qY * qZ; 
}