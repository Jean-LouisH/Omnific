#include "Scripts.hpp"

void TestGame::quitOnEscapeKey(Lilliputian::ScriptingAPI& API)
{
	if (API.input().isKeyOnPress(API.input().KEY_ESCAPE))
	{
		API.window().quit();
	}
}