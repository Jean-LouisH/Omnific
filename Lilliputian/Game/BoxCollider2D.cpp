#include "BoxCollider2D.hpp"

Lilliputian::BoxCollider2D::BoxCollider2D(EntityID entityID)
{
	this->entityID = entityID;
}

Lilliputian::EntityID Lilliputian::BoxCollider2D::getEntityID()
{
	return this->entityID;
}