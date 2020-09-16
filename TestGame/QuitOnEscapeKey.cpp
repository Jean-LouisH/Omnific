#include "Scripts.hpp"

void TestGame::quitOnEscapeKey(Lilliputian::ScriptingAPIs& APIs)
{
	if (APIs.input().isKeyOnPress(APIs.input().KEY_ESCAPE))
	{
		APIs.window().quit();
	}
}