#include <Lilliputian.hpp>
#include  "SceneDefiner.hpp"

int main(int argc, char* argv[])
{
	Lilliputian::Engine lilliputian;

	lilliputian.addSceneDefiner(TestGame::defineAllScenes);
	lilliputian.run();
	return 0;
}