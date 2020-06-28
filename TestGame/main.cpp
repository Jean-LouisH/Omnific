#include <Lilliputian.hpp>
#include  "SceneDefiner.hpp"

int main(int argc, char* argv[])
{
	Lilliputian::Engine lilliputian;

	lilliputian.setGameTitle("Lilliputian TestGame");
	lilliputian.setWindowDimensions(480, 640);
	lilliputian.addSceneDefiner(TestGame::splashScreen);
	lilliputian.addSceneDefiner(TestGame::demoSelectionMenu);
	lilliputian.addSceneDefiner(TestGame::systemTests);
	lilliputian.addSceneDefiner(TestGame::pongClone);
	lilliputian.runGame();
	return 0;
}