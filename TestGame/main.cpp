#include <Lilliputian.hpp>
#include  "ScriptCompiler.hpp"

int main(int argc, char* argv[])
{
	Lilliputian::Engine lilliputian(
		"Lilliputian TestGame", 
		480,
		640,
		"SplashScreen"
	);

	lilliputian.addScriptCompiler(TestGame::compileAll);
	lilliputian.runGame();
	return 0;
}