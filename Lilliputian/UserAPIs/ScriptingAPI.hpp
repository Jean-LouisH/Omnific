#pragma once

#include "GameAPI.hpp"
#include "InputAPI.hpp"

namespace Lilliputian
{
	class ScriptingAPI
	{
	private:
		GameAPI* gameAPI;
		InputAPI* inputAPI;
	public:
		ScriptingAPI(GameAPI* gameAPI, InputAPI* inputAPI);
		GameAPI& game() const;
		InputAPI& input() const;
	};
}

