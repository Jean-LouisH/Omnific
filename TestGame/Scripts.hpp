#include <Lilliputian.hpp>

namespace TestGame
{
	void quitOnEscapeKey(Lilliputian::ScriptingAPIs& APIs);
	void returnToMainMenu(Lilliputian::ScriptingAPIs& APIs);

	namespace PongClone
	{
		void controlAI(Lilliputian::ScriptingAPIs& APIs);
		void controlPlayer(Lilliputian::ScriptingAPIs& APIs);
		void generateBall(Lilliputian::ScriptingAPIs& APIs);
		void manageScore(Lilliputian::ScriptingAPIs& APIs);
		void shakeCamera(Lilliputian::ScriptingAPIs& APIs);
	}
}