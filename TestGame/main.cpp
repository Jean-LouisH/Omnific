#include <Lilliputian.hpp>
#include  "ScriptCompiler.hpp"

int main(int argc, char* argv[])
{
	Lilliputian::Engine lilliputian(
		"Lilliputian TestGame", 
		480,
		640,
		"Scenes/SplashScreen.yml"
	);

	lilliputian.addScriptCompiler(TestGame::compileAll);
	lilliputian.run();
	return 0;
}