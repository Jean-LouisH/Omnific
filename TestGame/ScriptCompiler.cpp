#include "ScriptCompiler.hpp"
#include "Scripts/Scripts.hpp"

void TestGame::compileAll(Lilliputian::ScriptRegistry* scriptRegistry)
{
	Lilliputian::Script returnToMainMenuScript = scriptRegistry->createNewScript("ReturnToMainMenu");
	Lilliputian::Script pongGenerateBallScript = scriptRegistry->createNewScript("PongGenerateBall");
	Lilliputian::Script pongManageScoreScript = scriptRegistry->createNewScript("PongManageScore");
	Lilliputian::Script pongShakeCameraScript = scriptRegistry->createNewScript("PongShakeCamera");
	Lilliputian::Script pongControlPlayerScript = scriptRegistry->createNewScript("PongControlPlayer");
	Lilliputian::Script pongControlAIScript = scriptRegistry->createNewScript("PongControlAI");

	returnToMainMenuScript.addFrameLogic(TestGame::returnToMainMenu);
	pongGenerateBallScript.addFrameLogic(PongClone::generateBall);
	pongManageScoreScript.addFrameLogic(PongClone::manageScore);
	pongShakeCameraScript.addFrameLogic(PongClone::shakeCamera);
	pongControlPlayerScript.addFrameLogic(PongClone::controlPlayer);
	pongControlAIScript.addFrameLogic(PongClone::controlAI);
}