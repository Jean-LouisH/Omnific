#include "AudioListener2D.hpp"

Lilliputian::AudioListener2D::AudioListener2D(EntityID entityID)
{
	this->entityID = entityID;
}

Lilliputian::EntityID Lilliputian::AudioListener2D::getEntityID()
{
	return this->entityID;
}