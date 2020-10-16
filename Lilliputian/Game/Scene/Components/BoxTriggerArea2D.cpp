#include "BoxTriggerArea2D.hpp"

bool Lilliputian::BoxTriggerArea2D::hasEntityEntered(EntityID entityID)
{
	return this->enteringEntityIDs.count(entityID);
}

bool Lilliputian::BoxTriggerArea2D::hasEntityWithTagEntered(String entityTag)
{
	return this->enteringEntityTags.count(entityTag);
}