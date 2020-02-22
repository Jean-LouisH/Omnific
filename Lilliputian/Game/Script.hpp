#pragma once

#include "../UserAPIs/ScriptingAPI.hpp"

namespace Lilliputian
{
	using ScriptLogicCallback = void(*)(Lilliputian::ScriptingAPI*);

	class Script
	{
	private:
		ScriptingAPI* scriptingAPI;

		ScriptLogicCallback startLogic;
		ScriptLogicCallback inputLogic;
		ScriptLogicCallback frameLogic;
		ScriptLogicCallback computeLogic;
		ScriptLogicCallback lateLogic;
		ScriptLogicCallback finalLogic;
	public:
		Script(ScriptingAPI* scriptingAPI);
		void addStartLogic(ScriptLogicCallback startLogic);
		void addInputLogic(ScriptLogicCallback inputLogic);
		void addFrameLogic(ScriptLogicCallback frameLogic);
		void addComputeLogic(ScriptLogicCallback computeLogic);
		void addLateLogic(ScriptLogicCallback lateLogic);
		void addFinalLogic(ScriptLogicCallback finalLogic);

		void executeStartLogic();
		void executeInputLogic();
		void executeFrameLogic();
		void executeComputeLogic();
		void executeLateLogic();
		void executeFinalLogic();
	};
}