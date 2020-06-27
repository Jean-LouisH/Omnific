#include "SceneDefiner.hpp"
#include "QuitOnEscapeKey.hpp"
#include "ReturnToMenu.hpp"
#include "PongClone/PongClone.hpp"

void TestGame::splashScreen(Lilliputian::EditorAPI* editor)
{
	Lilliputian::Scene mainScene = editor->createNewScene();
	Lilliputian::Entity2D rootEntity = editor->createNewEntity2D();
	Lilliputian::Entity2D camera = editor->createNewEntity2D();

	mainScene.addEntity2D(rootEntity);
	mainScene.addEntity2D(camera);
	editor->setEntryScene(mainScene);
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
	Lilliputian::Script returnToMenuScript = editor->createNewScript();
	Lilliputian::Script quitScript = editor->createNewScript();
	Lilliputian::Entity2D systemEntity = editor->createNewEntity2D();
	
	quitScript.addFrameLogic(TestGame::quitOnEscapeKey);
	systemEntity.addScript(quitScript);
	scene.addEntity2D(systemEntity);
	editor->addScene(scene);
}

void TestGame::pongClone(Lilliputian::EditorAPI* editor)
{
	Lilliputian::Scene scene = editor->createNewScene();
	Lilliputian::Entity2D rootEntity = editor->createNewEntity2D();
	Lilliputian::Script returnToMenuScript = editor->createNewScript();

	Lilliputian::Entity2D gameMechanicsEntity = editor->createNewEntity2D();
	Lilliputian::Entity2D cameraEntity = editor->createNewEntity2D();
	Lilliputian::Entity2D paddle1 = editor->createNewEntity2D();
	Lilliputian::Entity2D paddle2 = editor->createNewEntity2D();
	Lilliputian::Entity2D upperBoundary = editor->createNewEntity2D();
	Lilliputian::Entity2D lowerBoundary = editor->createNewEntity2D();
	Lilliputian::Entity2D scoreCount1 = editor->createNewEntity2D();
	Lilliputian::Entity2D scoreCount2 = editor->createNewEntity2D();
	Lilliputian::Script ballGenerationScript = editor->createNewScript();
	Lilliputian::Script scoringScript = editor->createNewScript();
	Lilliputian::Script cameraShakeScript = editor->createNewScript();
	Lilliputian::Script playerControlScript = editor->createNewScript();
	Lilliputian::Script aiControlScript = editor->createNewScript();

	returnToMenuScript.addFrameLogic(TestGame::returnToMenu);
	ballGenerationScript.addFrameLogic(PongClone::generateBall);
	scoringScript.addFrameLogic(PongClone::manageScore);
	cameraShakeScript.addFrameLogic(PongClone::shakeCamera);
	playerControlScript.addFrameLogic(PongClone::controlPlayer);
	aiControlScript.addFrameLogic(PongClone::controlAI);
	rootEntity.addScript(returnToMenuScript);
	gameMechanicsEntity.addScript(ballGenerationScript);
	gameMechanicsEntity.addScript(scoringScript);
	cameraEntity.addScript(cameraShakeScript);
	paddle1.addScript(playerControlScript);
	paddle2.addScript(aiControlScript);
	paddle1.addChildEntity(scoreCount1);
	paddle2.addChildEntity(scoreCount2);
	rootEntity.addChildEntity(gameMechanicsEntity);
	rootEntity.addChildEntity(cameraEntity);
	rootEntity.addChildEntity(paddle1);
	rootEntity.addChildEntity(paddle2);
	rootEntity.addChildEntity(upperBoundary);
	rootEntity.addChildEntity(lowerBoundary);
	scene.addEntity2D(rootEntity);
	editor->addScene(scene);
}