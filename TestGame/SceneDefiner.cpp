#include "SceneDefiner.hpp"
#include "QuitOnEscapeKey.hpp"

void TestGame::splashScreen(Lilliputian::EditorAPI* editor)
{
	Lilliputian::Scene mainScene = editor->createNewScene();
	Lilliputian::Entity2D rootEntity = editor->createNewEntity2D();

	mainScene.addEntity2D(rootEntity);
	editor->setMainScene(mainScene);
}

void TestGame::demoSelectionMenu(Lilliputian::EditorAPI* editor)
{
	Lilliputian::Scene scene = editor->createNewScene();
	Lilliputian::Entity2D rootEntity = editor->createNewEntity2D();

	scene.addEntity2D(rootEntity);
	editor->addScene(scene);
}

void TestGame::systemTests(Lilliputian::EditorAPI* editor)
{
	Lilliputian::Scene scene = editor->createNewScene();
	Lilliputian::Script quitScript = editor->createNewScript();
	Lilliputian::Entity2D systemEntity = editor->createNewEntity2D();
	
	quitScript.addFrameLogic(TestGame::quitOnEscapeKey);
	systemEntity.addScript(quitScript);
	scene.addEntity2D(systemEntity);
	editor->addScene(scene);
}