#include <Lilliputian.hpp>
#include  "SceneDefiner.hpp"

#ifdef main
#undef main
#endif

int main(int argc, char* argv[])
{
	Lilliputian::Engine lilliputian;

	lilliputian.setGameTitle("Lilliputian Demo");
	lilliputian.setWindowDimensions(480, 640);
	lilliputian.addSceneDefiner(TestGame::splashScreen);
	lilliputian.addSceneDefiner(TestGame::demoSelectionMenu);
	lilliputian.addSceneDefiner(TestGame::systemTests);
	lilliputian.addSceneDefiner(TestGame::pongClone);
	lilliputian.run();
	return 0;
}