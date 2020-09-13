#include <Lilliputian.hpp>
#include  "ScriptCompiler.hpp"

int main(int argc, char* argv[])
{
	Lilliputian::Engine lilliputian(
		argc,
		argv,
		"Lilliputian GameTemplate", 
		480,
		640,
		"Scenes/SplashScreen.yml"
	);

	lilliputian.addScriptCompilerFunction(TestGame::compileAll);
	lilliputian.run();
	return 0;
}