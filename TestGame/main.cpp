#include <Lilliputian.hpp>
#include  "ScriptCompiler.hpp"

int main(int argc, char* argv[])
{
	Lilliputian::Engine lilliputian;

	lilliputian.setGameTitle("Lilliputian TestGame");
	lilliputian.setWindowDimensions(480, 640);
	lilliputian.setEntryScene("SplashScreen");
	lilliputian.addScriptCompiler(TestGame::compileAll);
	lilliputian.runGame();
	return 0;
}