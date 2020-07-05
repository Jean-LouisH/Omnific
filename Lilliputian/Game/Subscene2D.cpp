#include "Subscene2D.hpp"

Lilliputian::Subscene2D::Subscene2D()
{

}

void Lilliputian::Subscene2D::addEntity2D(Entity2D entity2D)
{
	this->entities2D.push_back(entity2D);
}

void Lilliputian::Subscene2D::executeFrameLogic()
{
	for (int i = 0; i < this->entities2D.size(); i++)
	{

	}
}