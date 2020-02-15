#include "Editor.hpp"

Lilliputian::Editor::Editor()
{
	this->scriptingAPI = NULL;
}

Lilliputian::Scene2D Lilliputian::Editor::createNewScene2D()
{
	return Scene2D();
}

Lilliputian::Script Lilliputian::Editor::createNewScript()
{
	return Script(this->scriptingAPI);
}

Lilliputian::Entity2D Lilliputian::Editor::createNewEntity2D()
{
	return Entity2D();
}

void Lilliputian::Editor::setMainScene2D(Scene2D scene2D)
{
	this->mainScene2D = scene2D;
}