#include "Editor.hpp"

Lilliputian::Editor::Editor(OS* os)
{
	this->os = os;
}

Lilliputian::Scene Lilliputian::Editor::loadProjectList()
{
	Scene projectListScene;
	return projectListScene;
}

Lilliputian::Scene Lilliputian::Editor::loadMainUI()
{
	Scene mainUIScene;
	return mainUIScene;
}

void Lilliputian::Editor::exportProject(String filepath)
{

}