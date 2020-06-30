#pragma once

#include "GameAPI.hpp"
#include "InputAPI.hpp"

namespace Lilliputian
{
	class ScriptingAPI
	{
	private:
	public:
		GameAPI* game;
		InputAPI* input;

		ScriptingAPI(GameAPI* gameAPI, InputAPI* inputAPI);
	};
}

