#include "Scene2D.hpp"

Lilliputian::Scene2D::Scene2D()
{

}

void Lilliputian::Scene2D::addEntity2D(Entity2D entity2D)
{
	this->entities2D.push_back(entity2D);
}