#include "QuitOnEscapeKey.hpp"

void TestGame::quitOnEscapeKey(Lilliputian::ScriptingAPI* API)
{
	if (API->input.isKeyPressed(API->input.KEY_CODES_ESCAPE))
	{
		API->game.quit();
	}
}