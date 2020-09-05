#include <Lilliputian.hpp>

namespace TestGame
{
	void quitOnEscapeKey(Lilliputian::ScriptingAPI& API);
	void returnToMainMenu(Lilliputian::ScriptingAPI& API);

	namespace PongClone
	{
		void controlAI(Lilliputian::ScriptingAPI& API);
		void controlPlayer(Lilliputian::ScriptingAPI& API);
		void generateBall(Lilliputian::ScriptingAPI& API);
		void manageScore(Lilliputian::ScriptingAPI& API);
		void shakeCamera(Lilliputian::ScriptingAPI& API);
	}
}