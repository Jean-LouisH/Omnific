#include "Camera2D.hpp"

Lilliputian::Camera2D::Camera2D(EntityID entityID)
{
	this->entityID = entityID;
}

Lilliputian::EntityID Lilliputian::Camera2D::getEntityID()
{
	return this->entityID;
}