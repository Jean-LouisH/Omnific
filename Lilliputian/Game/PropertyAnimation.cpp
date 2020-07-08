#include "PropertyAnimation.hpp"

Lilliputian::PropertyAnimation::PropertyAnimation(EntityID entityID)
{
	this->targetDoubleProperty = NULL;
	this->keyFrames.clear();
	this->duration_s = 0.0;
	this->playbackSpeed_pct = 1.0;
	this->delay_s = 0.0;
	this->progress_s = 0.0;
	this->maximumValue = 0.0;
	this->minimumValue = 0.0;
	this->repeats = 0;
	this->repeatCount = 0;
	this->isPlaying = false;

	this->entityID = entityID;
}

Lilliputian::EntityID Lilliputian::PropertyAnimation::getEntityID()
{
	return this->entityID;
}