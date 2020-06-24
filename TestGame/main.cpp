#include <Lilliputian.hpp>
#include  "SceneDefiner.hpp"

int main(int argc, char* argv[])
{
	Lilliputian::Engine lilliputian;

	lilliputian.addSceneDefiner(TestGame::splashScreen);
	lilliputian.addSceneDefiner(TestGame::demoSelectionMenu);
	lilliputian.addSceneDefiner(TestGame::systemTests);
	lilliputian.addSceneDefiner(TestGame::pongClone);
	lilliputian.run();
	return 0;
}