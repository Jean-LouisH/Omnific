#include "rectangular_trigger_area_2d.hpp"

bool Lilliputian::RectangularTriggerArea2D::hasEntityEntered(EntityID entityID)
{
	return this->enteringEntityIDs.count(entityID);
}

bool Lilliputian::RectangularTriggerArea2D::hasEntityWithTagEntered(String entityTag)
{
	return this->enteringEntityTags.count(entityTag);
}