#include "SceneDefiner.hpp"
#include "QuitOnEscapeKey.hpp"

void TestGame::defineAllScenes(Lilliputian::Editor* editor)
{
	Lilliputian::Scene mainScene = editor->createNewScene();
	Lilliputian::Script quitScript = editor->createNewScript();
	Lilliputian::Entity2D systemEntity = editor->createNewEntity2D();
	
	quitScript.addFrameLogic(TestGame::quitOnEscapeKey);
	systemEntity.addScript(quitScript);
	mainScene.addEntity2D(systemEntity);
	editor->setMainScene(mainScene);
}