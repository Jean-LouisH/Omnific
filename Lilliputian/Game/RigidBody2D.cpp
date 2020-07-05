#include "RigidBody2D.hpp"

Lilliputian::RigidBody2D::RigidBody2D(EntityID entityID)
{
	this->entityID = entityID;
	this->mass_kg = 1.0;
	this->friction_ratio = 1.0;
	this->angularDrag_ratio = 1.0;
	this->angularVelocity_rad_per_s = 0.0;
	this->gravity_scale = 1.0;
	this->elasticity_ratio = 0.01;
	this->drag_ratio.x = 0.0001;
	this->drag_ratio.y = 1.0;
}

Lilliputian::EntityID Lilliputian::RigidBody2D::getEntityID()
{
	return this->entityID;
}