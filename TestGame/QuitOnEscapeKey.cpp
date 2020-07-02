#include "Scripts.hpp"

void TestGame::quitOnEscapeKey(Lilliputian::ScriptingAPI* API)
{
	if (API->input->isKeyPressed(API->input->KEY_ESCAPE))
	{
		API->game->quit();
	}
}